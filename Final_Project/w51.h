/********************************************************
 * config.h
 *
 * this file provides public declarations for SER486
 * W51 (Wiznet W5100 Ethernet Controller) class library functions.
 *
 * Author:   Doug Sandy
 * Date:     4/18/2018
 * Revision: 1.0
 *
 * Copyright(C) 2018, Arizona State University
 * All rights reserved
 *
 */

#ifndef  W51_H
#define  W51_H

/* initialize the W5100 Ethernet network controller */
void           W5x_init(void);

/* configure the Ethernet contoller mac address, ip address, network gateway address,
* and network subnet mask from the specified parameters.
*/
unsigned char  W5x_config(unsigned char*mac_addr, unsigned char *ip_addr, unsigned char *gtw_addr, unsigned char *sub_mask);

#endif
