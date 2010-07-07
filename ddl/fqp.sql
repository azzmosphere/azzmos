CREATE TABLE fqp (
	id        INTEGER PRIMARY KEY,
	scheme    INTEGER NOT NULL,
	authority INTEGER NOT NULL,
	uri       INTEGER NOT NULL,
	path      TEXT    NOT NULL,
	md5hash   CHAR(32),
	
	FOREIGN KEY(uri) REFERENCES uri(id),
	FOREIGN KEY(authority) REFERENCES authority(id),
	UNIQUE(authority,scheme,path)
);
CREATE SEQUENCE seq_fqp;
