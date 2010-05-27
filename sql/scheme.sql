/**
 * Create the scheme section of the database.
 *
 * The scheme section is the first section of the
 * URI which is used to define what type of application
 * the URI us for.  For instance http means Hyper Text
 * ftp means File Transfer Protocol.  cURL will use the
 * common ports for these protocols unless explicity told
 * not too.
 *
 * RFC 3986 states that the scheme is seperated fom the 
 * rest of the URI using the ':' character.  This is 
 * not recorded in the scheme_name attribute of the 
 * table and should be considered an error if it is.
 *
 */

DROP TABLE IF EXISTS az_scheme;
CREATE TABLE az_scheme (
	scheme_name  VARCHAR(10)  PRIMARY KEY
);
