/**
 * This code stores the URL headers for each URI. including the header 
 * header response.   
 */

CREATE TABLE az_url_headers (
	uri_id INTEGER,
	fqp_id INTEGER   NOT NULL, /* each URL gets a qualification before being stored. */
	hdr_id INTEGER   NOT NULL,
	cdate  TIMESTAMP NOT NULL DEFAULT now(),
	key    TEXT      NOT NULL,
	value  TEXT      NOT NULL
);
