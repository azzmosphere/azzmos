/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  upload_edge
 *  Description:  Upload two edges X an Y into the database.
 * =====================================================================================
 */

CREATE OR REPLACE FUNCTION upload_edge (
	xscheme TEXT,
	xcname  TEXT,
	xpath   TEXT,
	yscheme TEXT,
	ycname  TEXT,
	ypath   TEXT
)RETURNS INTEGER AS $$
DECLARE 
	xluri   INTEGER;
	yluri   INTEGER;
	edgeid INTEGER;
BEGIN
	SELECT upload_uri( xscheme, xcname, xpath), upload_uri( yscheme, ycname, ypath) 
	INTO xluri, yluri;
	
	SELECT id INTO edgeid FROM edges WHERE xuri = xluri AND yuri = yluri;
	
	IF edgeid IS NULL THEN
		INSERT INTO edges( id, xuri, yuri) VALUES( nextval('seq_edge'), xluri, yluri);
	END IF;

	RETURN yluri;	
END;
$$ LANGUAGE plpgsql;

