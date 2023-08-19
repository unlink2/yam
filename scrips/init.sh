#!/bin/bash

# rename to project specific values
NAME="yam"
CONST="YAM"
STRUCT="Yam"
FN="yam"

# will be replaced
YAM_NAME="yam"
YAM_STRUCT="Yam"
YAM_CONST="YAM"
YAM_FN="yam"

function replace() {
	echo "Replacing $1 with $2"
	find ./ -type f -name '*.c' -exec sed -i "s/$1/$2/g" {} \;
	find ./ -type f -name '*.h' -exec sed -i "s/$1/$2/g" {} \;
	find ./ -type f -name '*.md' -exec sed -i "s/$1/$2/g" {} \;
	find ./ -type f -name '*.lua' -exec sed -i "s/$1/$2/g" {} \;
	find ./ -type f -name '*.sh' -exec sed -i "s/$1/$2/g" {} \;
}

replace $YAM_NAME $NAME
replace $YAM_CONST $CONST
replace $YAM_STRUCT $STRUCT
replace $YAM_FN $FN

mv "src/lib$YAM_NAME" "src/lib$NAME"
mv "include/lib$YAM_NAME" "include/lib$NAME"
mv "include/lib$NAME/$YAM_NAME.h" "include/lib$NAME/$NAME.h"
