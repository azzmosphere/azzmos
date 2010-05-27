/**
 * The URI object contains the relationship with all attributes that
 * formulate a URI object.   For not the content of the URI object
 * only supports text but this should be changed so that images,
 * music, etc can be stored.
 */

DROP TABLE IF EXISTS az_uri;
CREATE TABLE az_uri (
	uri_id SERIAL PRIMARY KEY,
	az_scheme VARCHAR(10) NOT NULL, 
	az_auth   TEXT NOT NULL,        
	az_path   TEXT NOT NULL,
	az_content TEXT NOT NULL,

	CONSTRAINT fk_azuri_az_scheme FOREIGN KEY(az_scheme) REFERENCES az_scheme( scheme_name),
	CONSTRAINT fk_azuri_az_auth FOREIGN KEY(az_auth) REFERENCES az_authority( auth_name),
	CONSTRAINT idx_az_uri_uri UNIQUE( az_scheme, az_auth, az_path)
);

/**
 * the insert_az_uri() function is used to insert 
 * a new value into az_uri and update any subsequent tables
 *
 */
DROP FUNCTION IF EXISTS insert_az_uri(az_ischeme VARCHAR(10), az_iauth TEXT, az_ipath TEXT, az_icontent TEXT);
CREATE OR REPLACE FUNCTION insert_az_uri( az_ischeme VARCHAR(10), az_iauth TEXT, az_ipath TEXT, az_icontent TEXT )
RETURNS bool AS $$
DECLARE
	az_scheme_l  VARCHAR(10);	
	az_auth_l    TEXT;
	az_path_l    TEXT;
	az_content_l TEXT;
	uri_id_l     INTEGER;
BEGIN
	SELECT uri_id INTO uri_id_l FROM az_uri t
	WHERE
		az_scheme = az_ischeme AND
		az_auth   = az_iauth AND
		az_path   = az_ipath
	;
	IF FOUND THEN
		RAISE NOTICE 'uri all ready exists';
		RETURN FALSE;
	END IF;


	SELECT scheme_name INTO az_scheme_l FROM az_scheme WHERE scheme_name = az_ischeme;
	IF NOT FOUND THEN
		INSERT INTO az_scheme( scheme_name) VALUES( az_ischeme);
	END IF;

	SELECT auth_name INTO az_auth_l FROM az_authority where auth_name = az_iauth;

	IF NOT FOUND THEN
		INSERT INTO az_authority( auth_name ) VALUES ( az_iauth );
	END IF;

	INSERT INTO az_uri( 
		az_scheme,
		az_auth,
		az_path,
		az_content
	) VALUES (
		az_ischeme,
		az_iauth,
		az_ipath,
		az_icontent
	);

	RETURN TRUE;


END;
$$ LANGUAGE plpgsql;
