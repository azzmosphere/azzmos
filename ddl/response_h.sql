/*
 * =====================================================================================
 *
 *       Filename:  response_h.sql
 *
 *    Description:  Each time a new response is created a this table is automatically
 *                  updated to contain the original response.  This keeps a historic
 *                  reference of all previous reponses.
 *
 *        Version:  1.0
 *        Created:  05/07/2010 20:32:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */
CREATE TABLE response_h(
	id     INTEGER PRIMARY KEY,
	value  TEXT    NOT NULL,
	uri    INTEGER NOT NULL,
	FOREIGN KEY(uri)    REFERENCES uri(id)
);
