#include <crow.h>
#include <nlohmann/json.hpp>

int main() {
    crow::SimpleApp app;

    // Route to serve the questionnaire form
    CROW_ROUTE(app, "/questionnaire")
    ([]() {
        return R"(
            <!DOCTYPE html>
            <html>
            <head>
                <title>Questionnaire</title>
                <meta name="viewport" content="width=device-width, initial-scale=1">
                <style>
                    body { font-family: Arial, sans-serif; text-align: center; }
                    form { display: inline-block; text-align: left; margin-top: 20px; }
                    input, textarea { display: block; width: 100%; margin-bottom: 10px; padding: 8px; }
                    button { padding: 10px 20px; background-color: blue; color: white; border: none; cursor: pointer; }
                </style>
            </head>
            <body>
                <h2>Fill out this questionnaire</h2>
                <form action="/submit" method="POST">
                    <label for="name">Name:</label>
                    <input type="text" id="name" name="name" required>
                    
                    <label for="feedback">Feedback:</label>
                    <textarea id="feedback" name="feedback" required></textarea>

                    <button type="submit">Submit</button>
                </form>
            </body>
            </html>
        )";
    });

    // Route to handle form submission (POST request)
    CROW_ROUTE(app, "/submit").methods("POST"_method)
    ([](const crow::request& req) {
        auto name = req.body.find("name") != std::string::npos ? req.body.substr(req.body.find("name") + 5, req.body.find("feedback") - 6) : "Anonymous";
        auto feedback = req.body.find("feedback") != std::string::npos ? req.body.substr(req.body.find("feedback") + 9) : "No feedback given";

        // Create a JSON response
        nlohmann::json json_response;
        json_response["name"] = name;
        json_response["feedback"] = feedback;

        CROW_LOG_INFO << "New response: " << name << " - " << feedback;

        // Return a JSON response
        return crow::response{json_response.dump()};
    });

    app.bindaddr("0.0.0.0").port(5000).multithreaded().run();
}
