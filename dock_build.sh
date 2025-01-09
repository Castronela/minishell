#!/bin/bash

# Define the Docker image name and Dockerfile path
IMAGE_NAME="valgrind"
DOCKERFILE="Dockerfile"

# Function to get the creation date of the Docker image
get_image_creation_date() {
    docker inspect --format='{{.Created}}' "$1" 2>/dev/null
}

# Function to get the modification date of the Dockerfile
get_dockerfile_modification_date() {
    stat -c %Y "$1"
}

# Check if the Docker image exists
IMAGE_EXISTS=$(docker images -q "$IMAGE_NAME")

if [ -z "$IMAGE_EXISTS" ]; then
    echo "Docker image does not exist. Building the image..."
    docker build -t "$IMAGE_NAME" -f "$DOCKERFILE" .
else
    # Get the creation date of the Docker image
    IMAGE_CREATION_DATE=$(get_image_creation_date "$IMAGE_NAME")
    # Convert the creation date to a Unix timestamp
    IMAGE_CREATION_TIMESTAMP=$(date -d "$IMAGE_CREATION_DATE" +%s)

    # Get the modification date of the Dockerfile
    DOCKERFILE_MODIFICATION_TIMESTAMP=$(get_dockerfile_modification_date "$DOCKERFILE")

    # Compare the dates
    if [ "$DOCKERFILE_MODIFICATION_TIMESTAMP" -gt "$IMAGE_CREATION_TIMESTAMP" ]; then
        echo "Dockerfile is newer than the Docker image. Rebuilding the image..."
        docker build -t "$IMAGE_NAME" -f "$DOCKERFILE" .
    else
        echo "Docker image is up to date."
    fi
fi