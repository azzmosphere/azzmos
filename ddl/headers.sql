/*
 * =====================================================================================
 *
 *       Filename:  headers.sql
 *
 *    Description:  Downloaded headers get stored in this table.  There are
 *                  a series of headers that match one response. 
 *
 *        Version:  1.0
 *        Created:  05/07/2010 20:29:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri`
 *        Company:  
 *
 * =====================================================================================
 */
CREATE TABLE  headers (
	htypeid  INTEGER NOT NULL,
	cdate    TIMESTAMP NOT NULL DEFAULT now(),
	value    TEXT NOT NULL,
	response INTEGER NOT NULL,
	PRIMARY KEY( htypeid, response),
	FOREIGN KEY(htypeid)  REFERENCES header_types(id),
	FOREIGN KEY(response) REFERENCES response(id)
);

