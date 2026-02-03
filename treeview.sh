#!/usr/bin/env bash

# Config
ROOT="."
README="README.md"
[ -n "$1" ] && ROOT="$1"
[ -n "$2" ] && README="$2"

START_TAG=""
END_TAG=""

# 1. Generate the clean tree
TREE_CONTENT=$(tree -d -L 5 -I 'node_modules|.git|CMakeFiles|build|.pio' "$ROOT")

# 2. Extract the header (everything before the tag)
# We use 'grep -B' or a simple loop to avoid the sed error
TEMP_HEADER=$(mktemp)
while IFS= read -r line; do
    [[ "$line" == *"$START_TAG"* ]] && break
    echo "$line" >> "$TEMP_HEADER"
done < "$README"

# 3. Build the new README
{
    cat "$TEMP_HEADER"
    echo "$START_TAG"
    echo '```bash'
    echo "$TREE_CONTENT"
    echo '```'
    echo "$END_TAG"
} > "${README}.new"

mv "${README}.new" "$README"
rm "$TEMP_HEADER"

echo "✅ Treeview updated successfully in $README (Zero Sed Errors)."
