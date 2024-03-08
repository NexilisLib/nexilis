FROM mariadb:latest

# Set default values for build arguments
ARG MYSQL_HOST=localhost
ARG MYSQL_USER=root
ARG MYSQL_PASSWORD=my_password
ARG MYSQL_DATABASE=my_database

# Set environment variables
ENV MYSQL_DATABASE=$MYSQL_DATABASE
ENV MYSQL_ROOT_PASSWORD=$MYSQL_PASSWORD

# Expose port
EXPOSE 3306
