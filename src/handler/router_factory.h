#pragma once

#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/URI.h>

#include "auth_handler.h"
#include "metrics_handler.h"
#include "not_found_handler.h"
#include "user_handler.h"
#include "car_handler.h"
#include "rental_handler.h"
#include "../repository/user_repository.h"
#include "../repository/car_repository.h"
#include "../repository/rental_repository.h"

namespace handlers {

class RouterFactory : public Poco::Net::HTTPRequestHandlerFactory {
private:
    repository::UserRepository& userRepo;
    repository::CarRepository& carRepo;
    repository::RentalRepository& rentalRepo;
    
public:
    RouterFactory(repository::UserRepository& uRepo,
                  repository::CarRepository& cRepo,
                  repository::RentalRepository& rRepo)
        : userRepo(uRepo), carRepo(cRepo), rentalRepo(rRepo) {}
    
    Poco::Net::HTTPRequestHandler* createRequestHandler(
        const Poco::Net::HTTPServerRequest& request) override {
        const std::string& uri = request.getURI();
        const std::string& method = request.getMethod();

        if (uri == "/metrics" && method == "GET") {
            return new MetricsHandler();
        }
        
        if (uri == "/api/v1/auth" && method == "POST") {
            return new AuthHandler(userRepo);
        }
        
        if (uri == "/api/v1/users/register" && method == "POST") {
            return new RegisterUserHandler(userRepo);
        }
        if (uri.find("/api/v1/users/search") == 0 && method == "GET") {
            Poco::URI uriObj(uri);
            std::string login = getQueryParameter(uriObj, "login");
            std::string name = getQueryParameter(uriObj, "name");
            std::string surname = getQueryParameter(uriObj, "surname");
            
            if (!login.empty()) {
                return new SearchUserByLoginHandler(userRepo);}

            else if (!name.empty() || !surname.empty()) {
                return new SearchUserByMaskHandler(userRepo);
            }
        }
        
        if (uri == "/api/v1/cars" && method == "POST") {
            return new AddCarHandler(carRepo);
        }
        
        if (uri == "/api/v1/cars" && method == "GET") {
            return new GetAvailableCarsHandler(carRepo);
        }
        if (uri.find("/api/v1/cars/search") == 0 && method == "GET") {
            return new SearchCarsByClassHandler(carRepo);
        }
    
        if (uri == "/api/v1/rentals" && method == "POST") {
            return new CreateRentalHandler(rentalRepo, carRepo, userRepo);
        }

        if (uri == "/api/v1/rentals/active" && method == "GET") {
            return new GetActiveRentalsHandler(rentalRepo, carRepo);
        }
        
        if (uri == "/api/v1/rentals/history" && method == "GET") {
            return new GetRentalHistoryHandler(rentalRepo, carRepo);
        }

        if (uri.find("/api/v1/rentals/") != std::string::npos && 
            uri.find("/complete") != std::string::npos && 
            method == "PUT") {
            return new CompleteRentalHandler(rentalRepo, carRepo);
        }
        
        return new NotFoundHandler();
    }
};

} // namespace handlers