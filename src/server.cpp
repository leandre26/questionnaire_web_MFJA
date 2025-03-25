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
                    form { display: inline-block; text-align: left; margin-top: 20px; width: 300px; }
                    input, select, textarea { display: block; width: 100%; margin-bottom: 15px; padding: 8px; }
                    .radio-group { display: flex; flex-direction: column; margin-bottom: 15px; }
                    .radio-group label { display: flex; justify-content: space-between; align-items: center; margin-bottom: 5px; }
                    .radio-group input { margin-left: 10px; }
                    button { padding: 10px 20px; background-color: blue; color: white; border: none; cursor: pointer; }
                    h2 { margin-bottom: 20px; }
                </style>
            </head>
            <body>
                <h2>Remplissez ce questionnaire</h2>
                <form action="/submit" method="POST">
                    <label for="name">Nom :</label>
                    <input type="text" id="name" name="name" required>

                    <label for="group">Groupe :</label>
                    <input type="text" id="group" name="group" required>

                    <label for="date">Date :</label>
                    <input type="date" id="date" name="date" required>

                    <label for="machine">Référence Machine :</label>
                    <select id="machine" name="machine" required>
                        <option value="" disabled selected>Choisissez une machine</option>
                        <option value="DX200_1">DX200_1</option>
                        <option value="DX200_2">DX200_2</option>
                        <option value="DX200_3">DX200_3</option>
                        <option value="DX200_4">DX200_4</option>
                        <option value="DX200_5">DX200_5</option>
                        <option value="DX200_6">DX200_6</option>
                        <option value="DX200_EXP">DX200_EXP</option>
                        <option value="VX8_1">VX8_1</option>
                        <option value="VX8_2">VX8_2</option>
                        <option value="VX8_3">VX8_3</option>
                        <option value="VX8_4">VX8_4</option>
                        <option value="VX8_5">VX8_5</option>
                        <option value="VX8_6">VX8_6</option>
                        <option value="VX8_7">VX8_7</option>
                        <option value="VX8_8">VX8_8</option>
                        <option value="VX8_EXP">VX8_EXP</option>
                        <option value="AX200_1">AX200_1</option>
                        <option value="AX200_2">AX200_2</option>
                        <option value="AX200_MY">AX200_MY</option>
                        <option value="AX300_MSY">AX300_MSY</option>
                        <option value="UMILL5_15">UMILL5_15</option>
                        <option value="UMILL5_18">UMILL5_18</option>
                    </select>

                    <div class="radio-group">
                        <label>Niveau d'huile :</label>
                        <label>Haut <input type="radio" name="huile" value="Haut"></label>
                        <label>Moyen <input type="radio" name="huile" value="Moyen"></label>
                        <label>Bas <input type="radio" name="huile" value="Bas"></label>
                    </div>
                    
                    <div class="radio-group">
                        <label>Niveau lubrifiant :</label>
                        <label>Haut <input type="radio" name="lubrifiant" value="Haut"></label>
                        <label>Moyen <input type="radio" name="lubrifiant" value="Moyen"></label>
                        <label>Bas <input type="radio" name="lubrifiant" value="Bas"></label>
                    </div>
                    
                    <label for="temp">Température du bloc froid :</label>
                    <input type="number" id="temp" name="temp" step="0.1" required>
                    
                    <div class="radio-group">
                        <label>Niveau bac de copeaux :</label>
                        <label>Haut <input type="radio" name="bac" value="Haut"></label>
                        <label>Moyen <input type="radio" name="bac" value="Moyen"></label>
                        <label>Bas <input type="radio" name="bac" value="Bas"></label>
                    </div>
                    
                    <div class="radio-group">
                        <label>Tiroir haut desserte :</label>
                        <label>Complet <input type="radio" name="tiroir" value="Complet"></label>
                        <label>Manquant <input type="radio" name="tiroir" value="Manquant"></label>
                        <label>Vide <input type="radio" name="tiroir" value="Vide"></label>
                    </div>
                    
                    <label for="observation">Observation (optionnel) :</label>
                    <textarea id="observation" name="observation"></textarea>
                    
                    <button type="submit">Soumettre</button>
                </form>
            </body>
            </html>
        )";
    });
    
    // Route to handle form submission (POST request)
    CROW_ROUTE(app, "/submit").methods("POST"_method)
    ([](const crow::request& req) {
        auto json_data = nlohmann::json::parse(req.body, nullptr, false);
        if (json_data.is_discarded()) {
            return crow::response(400, "Invalid JSON");
        }
        CROW_LOG_INFO << "Nouvelle réponse: " << json_data.dump();
        return crow::response{json_data.dump()};
    });

    app.bindaddr("0.0.0.0").port(5000).multithreaded().run();
}
