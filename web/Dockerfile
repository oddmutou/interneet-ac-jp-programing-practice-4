FROM php:8.1

RUN apt update \
  && apt-get install -y git

COPY . /app

WORKDIR /app

COPY --from=composer /usr/bin/composer /usr/bin/composer

RUN composer install

CMD php -S 0.0.0.0:8089
