/**
 * Create the URL,  the URL is the primary place where
 * the contents of a webpage is stored.
 */

CREATE TABLE url (
	id       INTEGER   PRIMARY KEY,
	content  TEXT      NOT NULL,
	cdate    TIMESTAMP NOT NULL DEFAULT now()
);

CREATE SEQUENCE seq_url;
