/**
 * A footprint is left for each time a URI is visited.  If it is the first
 * visit to the website, then a new entry is created into the database.  
 * Otherwise the content is updated and the footprint is recorded.
 *
 * How much recursion has been done by the program to get to this point is 
 * recorded as well as 
 */

DROP TABLE IF EXISTS az_footprint;
CREATE TABLE az_footprint (
	foot_id SERIAL PRIMARY KEY,
	uri_id  INTEGER   NOT NULL,
	cdate   TIMESTAMP NOT NULL DEFAULT now(),
	recurse INTEGER   NOT NULL,

	CONSTRAINT fk_fprint_uri FOREIGN KEY(uri_id) REFERENCES az_uri(uri_id)
);

CREATE OR REPLACE FUNCTION insert_az_footprint( 
	az_ischeme  VARCHAR(10), 
	az_iauth    TEXT, 
	az_ipath    TEXT, 
	az_icontent TEXT,
        az_irecurse INTEGER
) RETURNS INTEGER AS $$
DECLARE
	r  RECORD;
	rv BOOL;
	az_uri_id_l  INTEGER;

BEGIN
	rv := FALSE;
	az_uri_id_l := 0;
	FOR r IN SELECT uri_id FROM az_uri WHERE 
		az_scheme = az_ischeme AND
		az_auth   = az_iauth   AND
		az_path   = az_ipath 
	LOOP
		az_uri_id_l := r.uri_id;		
	END LOOP;

	-- if there is an id then do an update
	IF NOT az_uri_id_l = 0 THEN
		UPDATE az_uri SET az_content = az_icontent WHERE uri_id = az_uri_id_l;
		rv := TRUE;
	END IF;

	-- if the URI does not exist create it.
	IF rv = FALSE THEN
		SELECT insert_az_uri(
			az_ischeme, 
			az_iauth,
			az_ipath,
			az_icontent
		) INTO rv;
		IF rv = true THEN
			FOR r IN SELECT uri_id FROM az_uri WHERE 
				az_scheme = az_ischeme AND
				az_auth   = az_iauth   AND
				az_path   = az_ipath 
			LOOP
				az_uri_id_l := r.uri_id;		
			END LOOP;
		ELSE
			RAISE EXCEPTION 'could not create URI';
		END IF;
	END IF;

	-- insert the footprint
	INSERT INTO az_footprint( 
		uri_id,
		recurse
	) VALUES (
		az_uri_id_l,
		az_irecurse
	);
	
	RETURN az_uri_id_l;
END
$$ LANGUAGE plpgsql;


	
