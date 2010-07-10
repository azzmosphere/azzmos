/* 
 * ===  TABLE  ======================================================================
 *         Name:  footprint
 *  Description:  Create a footprint (log entry of what has happened.) , this is 
 *                recorded with a status that matches the syslog.h status.
 * =====================================================================================
 */


DROP TABLE IF EXISTS footprint;
CREATE TABLE footprint (
	foot_id SERIAL PRIMARY KEY,
	status  INTEGER,
	cdate   TIMESTAMP NOT NULL DEFAULT now(),
	msg     TEXT
);


	
