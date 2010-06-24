#!/bin/bash
# Create the datbase schema.
#
# Change the value of username to user that has admin
# access to postgresql
#
USERNAME=azzmosphereadmin
DBNAME=azzmos

CREATEDB=`which createdb`
DROPDB=`which dropdb`
PSQL=`which psql`

# perform software varification
if [[ -z ${CREATEDB} ]]; then
	echo "createdb is not in your PATH"
	exit 1
fi

if [[ -z ${PSQL} ]]; then
	echo "psql is not in your PATH"
	exit 1
fi

if [[ -z ${DROPDB} ]];then
	echo "dropdb is not in your PATH"
	exit 1
fi

# create the database
sudo su ${USERNAME} -c "${DROPDB} ${DBNAME}"
sudo su ${USERNAME} -c "${CREATEDB} ${DBNAME}"

if [[ $? -ne 0 ]]; then
	echo "could not create the database"
fi

${PSQL} -f ./install.sql ${DBNAME} ${USERNAME}
if [[ $? -ne 0 ]]; then
	echo "could not create the database"
	exit 1
fi

exit 0
