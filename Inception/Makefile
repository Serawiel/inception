
COMPOSE_FILE = srcs/docker-compose.yml
DC = docker compose -f $(COMPOSE_FILE)
DATA_PATH = /home/cprot/data

all:
	mkdir -p $(DATA_PATH)/wordpress
	mkdir -p $(DATA_PATH)/mariadb
	$(DC) up -d --build

build:
	$(DC) build

up:
	mkdir -p $(DATA_PATH)/wordpress
	mkdir -p $(DATA_PATH)/mariadb
	$(DC) up -d

down:
	$(DC) down

stop:
	$(DC) stop

clean: down
	$(DC) down --rmi all

fclean: clean
	$(DC) down -v
	docker system prune -af --volumes

re: fclean all

.PHONY: all build up down stop clean fclean re