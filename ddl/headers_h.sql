/*
 * =====================================================================================
 *
 *       Filename:  headers_h.sql
 *
 *    Description:  headers_h is  a replication of the headers table except that
                    it keeps a history of past headers.
 *
 *        Version:  1.0
 *        Created:  05/07/2010 20:12:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri <azzmosphere@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
CREATE TABLE  headers_h (
	htypeid  INTEGER NOT NULL,
	cdate    TIMESTAMP NOT NULL DEFAULT now(),
	value    TEXT NOT NULL,
	response INTEGER NOT NULL,
	PRIMARY KEY( htypeid, response),
	FOREIGN KEY(htypeid)  REFERENCES header_types(id),
	FOREIGN KEY(response) REFERENCES response_h(id)
);

