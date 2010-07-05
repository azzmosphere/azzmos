/*
 * =====================================================================================
 *
 *       Filename:  ipv4.sql
 *
 *    Description: The IPv4 address is noted here in dotted decimal and is the address
 *                 that is returned by the DNS A record.  It is recorded in block format.
 *                 That is if we had the address 66.102.11.104 then the format would be 
 *                 b1 = 66, b2 = 102, b3 = 11 and b4 = 104.  The concardination of this
 *                 number is in dotted decimal format.
 *
 *        Version:  1.0
 *        Created:  05/07/2010 20:37:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */
CREATE TABLE ipv4(
	id        INTEGER PRIMARY KEY,
	b1        INTEGER DEFAULT 0,
	b2        INTEGER DEFAULT 0,
	b3        INTEGER DEFAULT 0,
	b4        INTEGER DEFAULT 0,
	authority INTEGER,
	FOREIGN KEY(authority) REFERENCES authority(id),
	UNIQUE(b1,b2,b3,b4)
);
CREATE SEQUENCE seq_ip;
