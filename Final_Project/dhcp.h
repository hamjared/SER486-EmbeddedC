/********************************************************
 * dhcp.h
 *
 * this file provides function declarations for SER486
 * dhcp connection functions.
 *
 * Author:   Doug Sandy
 * Date:     4/18/2018
 * Revision: 1.0
 *
 * Copyright(C) 2018, Arizona State University
 * All rights reserved
 *
 */
#ifndef DHCP_H_INCLUDED
#define DHCP_H_INCLUDED

/* typedef for IP addresses - a pointer to four unsigned char values */
typedef unsigned char* IPAddress;

/* attempt to get DHCP lease from the network's dhcp server.  A 1 is returned
* upon success.  Otherwise, 0 is returned.  Upon successful lease of a DHCP
* address, the network configuration information (IP addresses and netmask)
* can be obtained by using the DHCP get functions below
*/
int dhcp_start(unsigned char *mac, unsigned long timeout_ms, unsigned long responseTimeout_ms);

/* if a successful DHCP lease has been obtained, this function returns the
* value of the IP address leased to this host.  Otherwise, the value will
* be uninitialized and invalid.
*/
IPAddress dhcp_getLocalIp();

/* if a successful DHCP lease has been obtained, this function returns the
* value of the network subnet mask.  Otherwise, the value will
* be uninitialized and invalid.
*/
IPAddress dhcp_getSubnetMask();

/* if a successful DHCP lease has been obtained, this function returns the
* value of the network gateway's IP address.  Otherwise, the value will
* be uninitialized and invalid.
*/
IPAddress dhcp_getGatewayIp();

/* if a successful DHCP lease has been obtained, this function returns the
* value of the network Dhcp server's IP address.  Otherwise, the value will
* be uninitialized and invalid.
*/
IPAddress dhcp_getDhcpServerIp();

/* if a successful DHCP lease has been obtained, this function returns the
* value of the network Dns server's IP address.  Otherwise, the value will
* be uninitialized and invalid.
*/
IPAddress dhcp_getDnsServerIp();

/* Check to see if the host's dhcp lease is valid.  Returns 1 if the lease
* is valid, otherwise, returns 0
*/
int dhcp_checkLease();


#endif // DHCP_H_INCLUDED
