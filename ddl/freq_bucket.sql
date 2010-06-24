/**
 * freq_bucket,
 *
 * Keeps a time span of how frequently a URI is
 * updated.
 */

CREATE TABLE freq_bucket (
	id      INTEGER   PRIMARY KEY,
	boundl  INTERVAL  UNIQUE NOT NULL,
	boundu  INTERVAL  UNIQUE NOT NULL
);

CREATE SEQUENCE seq_freq_bucket;

/* Initial infinate timespan (by infinate this means 100 years)
 * ALl URIs get placed into this buket when they are first 
 * created and then moved by azzmos as they are checked.
 */
INSERT INTO freq_bucket(id, boundl, boundu) 
VALUES(
	nextval('seq_freq_bucket'),
	'3650 00:00:00',
	'36500 00:00:00'
);
