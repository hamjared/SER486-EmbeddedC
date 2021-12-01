#include "httpfsm.h"
#include "vpd.h"
#include "socket.h"
#include "temp.h"
#include "config.h"
#include "log.h"
#include "temputil.h"
#include "wdt.h"

#define SOCKET_NUM 0 // the socket number to be used for processing http requests

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

#define UPDATE_TCRIT_HI 0
#define UPDATE_TWARN_HI 1
#define UPDATE_TCRIT_LO 2
#define UPDATE_TWARN_LO 3



static unsigned char state;

static unsigned char temp_value_to_update;

static int update_temp_value;

static unsigned char reset = 0;


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
    socket_writequotedstring(SOCKET_NUM,"state"); socket_writestr(SOCKET_NUM, ":"); socket_writequotedstring(SOCKET_NUM, " "); socket_writestr(SOCKET_NUM, ",");

    // write log
    socket_writequotedstring(SOCKET_NUM,"log"); socket_writestr(SOCKET_NUM, ":[");

    for(int i = 0; i < log_get_num_entries(); i++){
        unsigned long timestamp;
        unsigned char event ;
        log_get_record(i, &timestamp, &event);
        socket_writestr(SOCKET_NUM, "{");
        socket_writequotedstring(SOCKET_NUM,"timestamp"); socket_writestr(SOCKET_NUM, ":"); socket_writedate(SOCKET_NUM, timestamp); socket_writestr(SOCKET_NUM, ",");

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

static void httpfsm_send_ok_response(){
    socket_writestr(SOCKET_NUM, "HTTP/1.1 200 OK\n");
    socket_writestr(SOCKET_NUM, "Connection: close\r\n");
    socket_writestr(SOCKET_NUM, "\r\n");
    socket_disconnect(SOCKET_NUM);
}


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
            temp_value_to_update = UPDATE_TCRIT_HI;
        }
        else if (socket_recv_compare(SOCKET_NUM, "twarn_hi=")){
            temp_value_to_update = UPDATE_TWARN_HI;
        }
        else if (socket_recv_compare(SOCKET_NUM, "tcrit_lo=")){
            temp_value_to_update = UPDATE_TCRIT_LO;
        }
        else if (socket_recv_compare(SOCKET_NUM, "twarn_lo=")){
            temp_value_to_update = UPDATE_TWARN_LO;
        }
        else{
            return 0;
        }

        //get the value that will be set, if the next value is not an int, this is not a valid request
        if(!socket_recv_int(SOCKET_NUM, &update_temp_value)){
            return 0;
        }


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

    return 1;
}

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

void httpfsm_update(){
    static REQUEST_TYPE request_type;

    switch(state){
    case INITIAL_STATE:
        reset = 0;
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

        state = INITIAL_STATE;
        break;
    }

}

void httpfsm_init(){
    state = INITIAL_STATE;
    reset = 0;
}

void httpfsm_reset(){
    state = FLUSH;
}
