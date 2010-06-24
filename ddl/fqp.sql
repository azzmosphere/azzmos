CREATE TABLE fqp (
	id        INTEGER PRIMARY KEY,
        uri       INTEGER NOT NULL,
	scheme    INTEGER NOT NULL,
	authority INTEGER NOT NULL,
	path      INTEGER NOT NULL,
	FOREIGN KEY(uri) REFERENCES uri(id),
	FOREIGN KEY(authority) REFERENCES authority(id),
	FOREIGN KEY(path) REFERENCES path(id),
	UNIQUE(authority,scheme,path)
);
CREATE CONSTRAINT seq_fqp;
