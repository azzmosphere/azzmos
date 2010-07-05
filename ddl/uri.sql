/*
 * Universal Resource Index
 */

CREATE TABLE uri (
	id           INTEGER   PRIMARY KEY,
	cdate        TIMESTAMP NOT NULL DEFAULT now(),
	last_checked TIMESTAMP NOT NULL,
	last_updated TIMESTAMP NOT NULL,
	recheck      BOOLEAN   NOT NULL DEFAULT false,
	importance   INTEGER   NOT NULL DEFAULT 0,
	url          INTEGER,
	fqp          INTEGER,
	freq_bucket  INTEGER,   
  	FOREIGN KEY(url)         REFERENCES url(id),
	FOREIGN KEY(freq_bucket) REFERENCES freq_bucket(id),
	FOREIGN KEY(fqp)         REFERENCES fqp(id)
);

CREATE SEQUENCE seq_uri;
