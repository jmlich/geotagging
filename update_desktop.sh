#!/bin/bash
query='string(//context[name="MainWindow"]/message[source="Geotagging"]/translation)'
desktop_file="com.github.jmlich.geotagging.desktop"
repo=.
for fn in $(find $repo -name '*.ts' ! -name '*depend*' -type f); do
    lang=$(xmllint --xpath 'string(//TS/@language)' "$fn") #'
    if [ -z "$lang" ]; then
        continue
    fi
    local_name=$(xmllint --xpath "$query" "$fn")

    if ! grep -q "^Name\[$lang\]=" $desktop_file; then
        echo "Appending $desktop_file / $lang"
        echo "Name[$lang]=$local_name" >> $desktop_file
    else
        sed 's/^Name\['"$lang"'\]=.*/Name['"$lang"']='"$local_name"'/g' -i "$desktop_file"
    fi

done
