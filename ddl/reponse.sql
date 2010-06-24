CREATE TABLE response(
	id     PRIMARY KEY,
	value  TEXT NOT NULL,
	hdrgrp INTEGER NOT NULL,
	uri    INTEGER NOT NULL,
	FOREIGN KEY(hdrgrp) REFERENCE header(hdrgrpid),
	FOREIGN KEY(uri)    REFERENCES uri(id)
);
CREATE SEQUENCE seq_response;
