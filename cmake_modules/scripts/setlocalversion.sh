#!/bin/bash
# Print additional version information for non-release trees.

usage() {
	echo "Usage: $0 [--notime]" >&2
	exit 1
}

get_local_version() {
	# Check for git and a git repo.
	if head=`git rev-parse --verify HEAD 2>/dev/null`; then

        	# Do we have an untagged version?
        	if git name-rev --tags HEAD | grep -E '^HEAD[[:space:]]+(.*~[0-9]*|undefined)$' > /dev/null; then
                	printf '%s%s' git- `echo "$head" | cut -c1-8`
        	else
                	printf '%s' `git name-rev --tags HEAD | cut -f2 -d/ | cut -f1 -d^`
        	fi

        	# Are there uncommitted changes?
        	if git diff-index HEAD | read dummy; then
                	printf '%s' -dirty
        	fi
	else
        	echo -n "UNDEF"
	fi
}

##
# Check arguments
##

HEADER=0
TIME=1
for ((i=0; $i < $#; i=$i+1)); do
	if [ "$1" = "--notime" ]; then
		TIME=0
	elif [ "$1" = "--help" ]; then
		usage
	elif [ "$1" = "--lite" ]; then
		get_local_version
		exit 0
	elif [ "$1" = "--header" ]; then
		HEADER=1
	fi
	shift
done

if [ $HEADER -eq 1 ]; then
	echo "#ifndef __VERSION_H_"
	echo "#define __VERSION_H_"
	echo
else
	echo "#include \"version.h\""
	echo
fi

##
# Output compile time
##

if [ $TIME -ne 0 ]; then
	echo "const char COMPILE_TIME[] = __DATE__ \" \" __TIME__;"
fi

##
# Output repository version
##

echo -n "const char VERSION[] = \""

get_local_version

echo "\";"
echo

if [ $HEADER -eq 1 ]; then
	echo "#endif"
fi
