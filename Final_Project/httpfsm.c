/*************************************************************************
* httpfsm.c
*
* This code for implementing an http fsm which is used to receive and respond to
http requests received on socket 0.

Jared Ham
SER 486 final project
*/


#include "httpfsm.h"
#include "vpd.h"
#include "socket.h"
#include "temp.h"
#include "config.h"
#include "log.h"
#include "temputil.h"
#include "wdt.h"
#include "rtc.h"
#include "uart.h"

#define SOCKET_NUM 0 // the socket number to be used for processing http requests

// define states for the FSM
#define INITIAL_STATE 0
#define FLUSH 1
#define URI_PARSE 2
#define HEADERS 3
#define GET 4
#define PUT 5
#define DELETE 6
#define HTTP_VERSION 7
#define BODY 8
#define PROCESS_MESSAGE 9

#define REQUEST_TYPE unsigned char

// define variables to use to determine which temp variable to update on a put request
#define UPDATE_TCRIT_HI 0
#define UPDATE_TWARN_HI 1
#define UPDATE_TCRIT_LO 2
#define UPDATE_TWARN_LO 3


// fsm state
static unsigned char state;

// the temp value to update. Should be one of UPDATE_X defines
static unsigned char temp_value_to_update;

// the value received in the put request for updating one of the temp limits
static int update_temp_value;

// variable to determine whether or not to rest device after http request has been verified.
static unsigned char reset = 0;

/**
get_temp_state()()

This function returns the current state of the device by comparing the current temperature to the
temperature limits defined in config


returns:
    current state of the device
changes:
    NONE
NOTE:
    This is intended to be a private method of this "class"
*/
static char * get_temp_state(){
    int temp = temp_get();
    if(temp >= config.hi_alarm){
        return  "CRIT_HI";
    }
    else if(temp >= config.hi_warn){
        return "WARN_HI";
    }
    else if(temp <= config.lo_alarm){
        return  "CRIT_LO";
    }
    else if(temp <= config.lo_warn){
        return  "WARN_LO";
    }
    else {
        return  "NORMAL";
    }
}

/**
get_temp_state()()

This function sends the response for an HTTP get which is a json object of the current device state


returns:
    nothing
changes:
    The contents of the socket 0 write buffer
    Closes the socket 0 write buffer before returning.
NOTE:
    This is intended to be a private method of this "class"
*/
static void httpfsm_send_get_response(){
    // write http response code
    socket_writestr(SOCKET_NUM, "HTTP/1.1 200 OK\n");

    //write headers
    socket_writestr(SOCKET_NUM, "Content-Type: application/vnd.api+json\r\n");
    socket_writestr(SOCKET_NUM, "Connection: close\r\n");
    socket_writestr(SOCKET_NUM, "\r\n");

    // write json body
    socket_writestr(SOCKET_NUM, "{");

    // write the vpd data
    socket_writequotedstring(SOCKET_NUM, "vpd"); socket_writestr(SOCKET_NUM, ":{");
    socket_writequotedstring(SOCKET_NUM,"model"); socket_writestr(SOCKET_NUM, ":"); socket_writequotedstring(SOCKET_NUM, vpd.model); socket_writestr(SOCKET_NUM, ",");
    socket_writequotedstring(SOCKET_NUM,"manufacturer"); socket_writestr(SOCKET_NUM, ":"); socket_writequotedstring(SOCKET_NUM, vpd.manufacturer); socket_writestr(SOCKET_NUM, ",");
    socket_writequotedstring(SOCKET_NUM,"serial_number"); socket_writestr(SOCKET_NUM, ":"); socket_writequotedstring(SOCKET_NUM, vpd.serial_number); socket_writestr(SOCKET_NUM, ",");
    socket_writequotedstring(SOCKET_NUM,"manufacture_date"); socket_writestr(SOCKET_NUM, ":"); socket_writedate(SOCKET_NUM, vpd.manufacture_date); socket_writestr(SOCKET_NUM, ",");
    socket_writequotedstring(SOCKET_NUM,"mac_address"); socket_writestr(SOCKET_NUM, ":"); socket_write_macaddress(SOCKET_NUM, vpd.mac_address); socket_writestr(SOCKET_NUM, ",");
    socket_writequotedstring(SOCKET_NUM,"country_code"); socket_writestr(SOCKET_NUM, ":"); socket_writequotedstring(SOCKET_NUM, vpd.country_of_origin);
    socket_writestr(SOCKET_NUM, "},");

    // write the temp alarm values
    socket_writequotedstring(SOCKET_NUM,"tcrit_hi"); socket_writestr(SOCKET_NUM, ":"); socket_writedec32(SOCKET_NUM, config.hi_alarm); socket_writestr(SOCKET_NUM, ",");
    socket_writequotedstring(SOCKET_NUM,"twarn_hi"); socket_writestr(SOCKET_NUM, ":"); socket_writedec32(SOCKET_NUM, config.hi_warn); socket_writestr(SOCKET_NUM, ",");
    socket_writequotedstring(SOCKET_NUM,"tcrit_lo"); socket_writestr(SOCKET_NUM, ":"); socket_writedec32(SOCKET_NUM, config.lo_alarm); socket_writestr(SOCKET_NUM, ",");
    socket_writequotedstring(SOCKET_NUM,"twarn_lo"); socket_writestr(SOCKET_NUM, ":"); socket_writedec32(SOCKET_NUM, config.lo_warn); socket_writestr(SOCKET_NUM, ",");

    // write cur temp
    socket_writequotedstring(SOCKET_NUM,"temperature"); socket_writestr(SOCKET_NUM, ":"); socket_writedec32(SOCKET_NUM, temp_get()); socket_writestr(SOCKET_NUM, ",");

    //write state
    socket_writequotedstring(SOCKET_NUM,"state"); socket_writestr(SOCKET_NUM, ":"); socket_writequotedstring(SOCKET_NUM, get_temp_state()); socket_writestr(SOCKET_NUM, ",");

    // write log
    socket_writequotedstring(SOCKET_NUM,"log"); socket_writestr(SOCKET_NUM, ":[");

    for(int i = 0; i < log_get_num_entries(); i++){
        unsigned long timestamp;
        unsigned char event ;
        log_get_record(i, &timestamp, &event);
        socket_writestr(SOCKET_NUM, "{");
        socket_writequotedstring(SOCKET_NUM,"timestamp"); socket_writestr(SOCKET_NUM, ":"); socket_writequotedstring(SOCKET_NUM, rtc_num2datestr(timestamp)); socket_writestr(SOCKET_NUM, ",");

        socket_writequotedstring(SOCKET_NUM,"event"); socket_writestr(SOCKET_NUM, ":"); socket_writedec32(SOCKET_NUM, event);
        socket_writestr(SOCKET_NUM, "}");

        if(i != log_get_num_entries() - 1){
            socket_writestr(SOCKET_NUM, ",");
        }
    }

    socket_writestr(SOCKET_NUM, "]}");

    //write final CRLF
    socket_writestr(SOCKET_NUM, "\r\n");

    //disconnect the socket
    socket_disconnect(SOCKET_NUM);
}

/**
get_temp_state()()

This function sends an HTTP OK response and disconnects the socket 0 upon completion

returns:
    nothing
changes:
    The contents of the socket 0 write buffer
    Closes the socket 0 write buffer before returning.
NOTE:
    This is intended to be a private method of this "class"
*/
static void httpfsm_send_ok_response(){
    socket_writestr(SOCKET_NUM, "HTTP/1.1 200 OK\n");
    socket_writestr(SOCKET_NUM, "Connection: close\r\n");
    socket_writestr(SOCKET_NUM, "\r\n");
    socket_disconnect(SOCKET_NUM);
}

/**
get_temp_state()()

This function sends the HTTP 400 response


returns:
    nothing
changes:
    The contents of the socket 0 write buffer
    Closes the socket 0 write buffer before returning.
NOTE:
    This is intended to be a private method of this "class"
*/
static void httpfsm_send_error_response(){
    socket_writestr(SOCKET_NUM, "HTTP/1.1 400 Bad Request\n");
    socket_writestr(SOCKET_NUM, "Connection: close\r\n");
    socket_writestr(SOCKET_NUM, "\r\n");
    socket_disconnect(SOCKET_NUM);
}



/**
httpfsm_process_uri()

returns
    0 if the request is bad
    1 otherwise
*/
static unsigned char httpfsm_process_uri(){
    // throw away the characters up to the first /. Also set a limit to the characters that will be processed before the loop exits
    unsigned char MAX_CHARS_BEFORE_RESOURCE = 30; // this will limit the max number of chars to process before a / is found. If this is exceeded flush state will be entered
    unsigned char count = 0;
    unsigned char buf[1];
    while(count < MAX_CHARS_BEFORE_RESOURCE){
        socket_recv(SOCKET_NUM, buf, 1);
        if(buf[0] == '/'){
            break;
        }
        count++;
    }
    if(count>= MAX_CHARS_BEFORE_RESOURCE){
        // too many characters before resource return 0 indicating a bad request
        return 0;
    }

    if(!socket_recv_compare(SOCKET_NUM, "device")){
        // device is the only valid top level resource. If it is anything else this is a bad request
        return 0;
    }


    return 1;
}

/**
httpfsm_process_get()

returns
    0 if the request is bad
    1 otherwise
*/
static unsigned char httpfsm_process_get(){
    return httpfsm_process_uri();
}

/**
httpfsm_process_put()

returns
    0 if the request is bad
    1 otherwise
*/
static unsigned char httpfsm_process_put(){
    // check request up to /device
    if(httpfsm_process_uri() == 0){
        return 0;
    }

    if(socket_recv_compare(SOCKET_NUM, "/config?")){
        if(socket_recv_compare(SOCKET_NUM, "tcrit_hi=")){
            uart_writestr("Updating tcrit_hi with value: ");
            temp_value_to_update = UPDATE_TCRIT_HI;
        }
        else if (socket_recv_compare(SOCKET_NUM, "twarn_hi=")){
            uart_writestr("Updating twarn_hi with value: ");
            temp_value_to_update = UPDATE_TWARN_HI;
        }
        else if (socket_recv_compare(SOCKET_NUM, "tcrit_lo=")){
            uart_writestr("Updating tcrit_lo with value: ");
            temp_value_to_update = UPDATE_TCRIT_LO;
        }
        else if (socket_recv_compare(SOCKET_NUM, "twarn_lo=")){
            uart_writestr("Updating twarn_lo with value: ");
            temp_value_to_update = UPDATE_TWARN_LO;
        }
        else{
            return 0;
        }

        //get the value that will be set, if the next value is not an int, this is not a valid request
        if(!socket_recv_int(SOCKET_NUM, &update_temp_value)){
            return 0;
        }

        uart_writedec32((long)update_temp_value);
        uart_writestr("\r\n");


        // One valid resource for a put command is config
        return 1;
    }
    else if(socket_recv_compare(SOCKET_NUM, "?reset=")){
        if(socket_recv_compare(SOCKET_NUM, "\"false\"")){
            reset = 0;
        }
        else if(socket_recv_compare(SOCKET_NUM, "\"true\"")){
            reset = 1;
        }
        else{
            return 0;
        }
        // Another valid resource for a put command
        return 1;
    }

    // All valid cases are handled and return before this point,
    // if we make it here that means the request was invalid.
    return 0;
}

/**
httpfsm_process_delete()

returns
    0 if the request is bad
    1 otherwise
*/
static unsigned char httpfsm_process_delete(){
    if(httpfsm_process_uri() == 0){
        return 0;
    }

    if(!socket_recv_compare(SOCKET_NUM, "/log")){
        // the only valid resources for a delete command is log.
        // If this is not the case it is a bad request
        return 0;
    }

    unsigned char buf[2];
    socket_peek(SOCKET_NUM, buf);
    if(buf[0] != ' '){
        // there must be a space after /log otherwise an invalid uri was passed in for a delete request.
        return 0;
    }

    log_clear();


    return 1;
}

/**
httpfsm_do_put()

This function performs the action of an HTTP put request. Call this after verifying the
entire request was valid. It sends the corresponding HTTP request and closes socket 0 before returning


returns:
    nothing
changes:
    The config values for temperature limits
    The contents of the socket 0 write buffer
    Closes the socket 0 write buffer before returning.
NOTE:
    This is intended to be a private method of this "class"
*/
static void httpfsm_do_put(){
    if(reset > 0){
        httpfsm_send_ok_response();
        wdt_force_restart();
    }
    else{
        switch(temp_value_to_update){
        case UPDATE_TCRIT_HI:
            if(update_tcrit_hi(update_temp_value) == 0){
                httpfsm_send_ok_response();
            }
            else{
                httpfsm_send_error_response();
            }
            break;
        case UPDATE_TWARN_HI:
            if(update_twarn_hi(update_temp_value) == 0){
                httpfsm_send_ok_response();
            }
            else{
                httpfsm_send_error_response();
            }
            break;
        case UPDATE_TCRIT_LO:
            if(update_tcrit_lo(update_temp_value) == 0){
                httpfsm_send_ok_response();
            }
            else{
                httpfsm_send_error_response();
            }
            break;
        case UPDATE_TWARN_LO:
            if(update_twarn_lo(update_temp_value) == 0){
                httpfsm_send_ok_response();
            }
            else{
                httpfsm_send_error_response();
            }
            break;
        }
    }
}

/**
httpfsm_update()

This function updates the fsm for http requests.


returns:
    nothing
changes:
    The state of the socket 0 receive and write buffers

NOTE:
    call httpfsm_init() before calling this function.
*/
void httpfsm_update(){
    static REQUEST_TYPE request_type;
    switch(state){
    case INITIAL_STATE:
        if(socket_recv_compare(SOCKET_NUM, "GET")){
                request_type = GET;
                state = GET;
        }
        else if(socket_recv_compare(SOCKET_NUM, "PUT")){
                request_type = PUT;
                state = PUT;
        }
        else if(socket_recv_compare(SOCKET_NUM, "DELETE")){
                request_type = DELETE;
                state = DELETE;
        }
        else{
            // since the request did not start with GET, PUT or DELETE the request is invalid
            state = FLUSH;
        }
        break;
    case FLUSH:
        while(socket_recv_available(SOCKET_NUM)){
            socket_flush_line(SOCKET_NUM);
        }
        httpfsm_send_error_response();
        state = INITIAL_STATE;
        break;
    case GET:
        if(httpfsm_process_get() == 0){
            state = FLUSH;
        }
        else{
            //httpfsm_send_get_response();
            state = HTTP_VERSION;
        }
        break;
    case PUT:
        if(httpfsm_process_put() == 0){
            state = FLUSH;
        }
        else{
            state = HTTP_VERSION;
        }
        break;
    case DELETE:
        if(httpfsm_process_delete() == 0){
            state = FLUSH;
        }
        else{
            state = HTTP_VERSION;
        }
        break;
    case HTTP_VERSION:
        if(!socket_recv_compare(SOCKET_NUM, " HTTP/1.1\r\n")){
            state = FLUSH;
        }
        else {
            state = HEADERS;
        }
        break;
    case HEADERS:
        while(1){
            if(socket_is_blank_line(SOCKET_NUM)){
                break;
            }
            // flush all the headers since we do not care about them
            socket_flush_line(SOCKET_NUM);
        }
        state = PROCESS_MESSAGE;
        break;
    case PROCESS_MESSAGE:
        if(request_type == GET){
            httpfsm_send_get_response();
        }
        else if (request_type == PUT){
            httpfsm_do_put();
        }
        else if (request_type == DELETE){
            httpfsm_send_ok_response();
        }

        state = INITIAL_STATE;
        break;
    }

}

/**
httpfsm_init()

This function initializes the httpfsm state machine


returns:
    nothing
*/
void httpfsm_init(){
    state = INITIAL_STATE;
    reset = 0;
}

/**
httpfsm_init()

This function resets the httpfsm state machine. It will flush the contents of socket 0 and then
return to the Intitial state


returns:
    nothing
*/
void httpfsm_reset(){
    state = FLUSH;
}
