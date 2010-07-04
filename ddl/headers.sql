CREATE TABLE  headers (
	htypeid  INTEGER NOT NULL,
	cdate    TIMESTAMP NOT NULL DEFAULT now(),
	value    TEXT NOT NULL,
	response INTEGER NOT NULL,
	PRIMARY KEY( htypeid, response),
	FOREIGN KEY(htypeid) REFERENCES header_types(id),
	FOREIGN KEY(reponse) REFERENCES reponse(id)
);
CREATE SEQUENCE seq_hdrgrpid;

