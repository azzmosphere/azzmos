/**
 * NOT CURRENTLY IMPLEMENTED.
 */

CREATE TABLE ip(
	id        INTEGER PRIMARY KEY,
	address   TEXT UNIQUE NOT NULL,
	ipv4      BOOLEAN,
	authority INTEGER,
	FOREIGN KEY(authority) REFERENCES authority(id)
);
