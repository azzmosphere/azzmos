/**
 * the authority section describes the Universal Resource Locations (URL),
 * All though this is not a requirement according to RFC 3986, for the 
 * purposes of azzmos it is allways a requirement.  If it is not avaible
 * it considered an error and is builded via the program before being 
 * used here.
 */

DROP TABLE IF EXISTS az_authority;
CREATE TABLE az_authority (
	auth_name TEXT PRIMARY KEY
);
