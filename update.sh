#!/usr/bin/env bash

# 1. Status repo (Optional/Manual check)
# git status

# 2. Check for changes
if [ -n "$(git status --porcelain)" ]; then
    echo "There are pending changes."
    
    # Add all changes
    git add .
    # manual tui
    # git add -i
    
    # Run your treeview script (to show the user what happened)
    ./treeview.sh
    git add .
    # Commit with date and a list of changed files
    # We use a variable for the status to keep the command clean
    CHANGES=$(git status --porcelain)
    git commit -m "updated repo $(date +%Y-%m-%d)" -m "$CHANGES"
    
    git push
else
    echo "Nothing to commit."
fi

# 3. Build the new README (Add your logic below)