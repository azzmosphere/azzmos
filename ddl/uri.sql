/*
 * =====================================================================================
 *
 *       Filename:  url.sql
 *
 *    Description:  Universal Resource Index
 *                  Each URL has at least one URI, in some cases they can
 *                  have more,  this can occur if it a duplicated site or
 *                  if there are several IP assosicated with the one CNAME.
 *
 *        Version:  1.0
 *        Created:  09/07/2010 23:01:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */


CREATE TABLE uri (
	id           INTEGER   PRIMARY KEY,
	cdate        TIMESTAMP NOT NULL DEFAULT now(),
	last_checked TIMESTAMP NOT NULL,
	last_updated TIMESTAMP,
	recheck      BOOLEAN   NOT NULL DEFAULT false,
	importance   INTEGER   NOT NULL DEFAULT 0,
	url          INTEGER,
	freq_bucket  INTEGER,   
  	FOREIGN KEY(url)         REFERENCES url(id),
	FOREIGN KEY(freq_bucket) REFERENCES freq_bucket(id)
);

CREATE SEQUENCE seq_uri;
