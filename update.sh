#!/usr/bin/env bash

set -e

# 1. Show the current repository tree
./treeview.sh

# 2. Check for changes
if [[ -n "$(git status --porcelain)" ]]; then
    echo "There are pending changes."

    # Stage all changes
    git add -A

    # Show the updated tree
    ./treeview.sh

    # Stage any changes made by treeview.sh
    git add -A

    # Capture staged changes for the commit body
    CHANGES=$(git diff --cached --name-status)

    # Get current date and time
    DATETIME=$(date '+%Y-%m-%d %H:%M:%S')

    # Build commit title
    if [[ -n "$1" ]]; then
        COMMIT_TITLE="updated repo $DATETIME - $1"
    else
        COMMIT_TITLE="updated repo $DATETIME"
    fi

    # Commit with date, time, optional comment, and changed files
    git commit \
        -m "$COMMIT_TITLE" \
        -m "Changed files:" \
        -m "$CHANGES"

    # Push the commit
    git push
else
    echo "Nothing to commit."
fi
