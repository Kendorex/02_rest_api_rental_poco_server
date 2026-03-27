# Poco Rental Server

REST сервер на C++ с использованием библиотеки POCO.

Дополнительно postman_collection 

## API Endpoints

### Auth
| Метод | Endpoint | Описание |
|-------|----------|----------|
| POST | `/api/v1/auth` | Получение JWT токена (Basic Auth) |

### Users
| Метод | Endpoint | Описание |
|-------|----------|----------|
| POST | `/api/v1/users/register` | Регистрация нового пользователя |
| GET | `/api/v1/users/search?login={username}` | Поиск пользователя по логину (admin) |
| GET | `/api/v1/users/search?name={mask}&surname={mask}` | Поиск по маске имени/фамилии (admin) |

### Cars
| Метод | Endpoint | Описание |
|-------|----------|----------|
| GET | `/api/v1/cars` | Список доступных автомобилей |
| POST | `/api/v1/cars` | Добавление нового автомобиля (fleetManager/admin) |
| GET | `/api/v1/cars/search?class={class}` | Поиск автомобилей по классу |

### Rentals
| Метод | Endpoint | Описание |
|-------|----------|----------|
| POST | `/api/v1/rentals` | Создание новой аренды (customer+) |
| GET | `/api/v1/rentals/active` | Получение активных аренд (customer+) |
| GET | `/api/v1/rentals/history` | Получение истории аренд (customer+) |
| PUT | `/api/v1/rentals/{id}/complete` | Досрочное завершение аренды (владелец/admin) |

## Сборка

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Docker

```bash
docker-compose up
```
docker run -p 8080:8080 -e LOG_LEVEL=debug poco_template_server
```
