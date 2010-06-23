/*
 * Universal Resource Index
 */

CREATE TABLE uri (
	id           INTEGER   PRIMARY KEY,
	cdate        TIMESTAMP NOT NULL DEFAULT now(),
	url          INTEGER,
	freq_bucket  INTEGER,   
  	FOREIGN KEY(url) REFERENCES url(id),
	FOREIGN KEY(freq_bucket) REFERENCES  freq_bucket(id)
);

CREATE SEQUENCE seq_uri;
