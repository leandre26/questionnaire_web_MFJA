#include <crow.h>
#include <nlohmann/json.hpp>

int main() {
    crow::SimpleApp app;

    // Route to serve the questionnaire form
    CROW_ROUTE(app, "/questionnaire")([]() {
        return R"(
            <!DOCTYPE html>
            <html>
            <head>
                <title>Questionnaire</title>
                <meta name="viewport" content="width=device-width, initial-scale=1">
                <style>
                    body { font-family: Arial, sans-serif; text-align: center; }
                    form { display: inline-block; text-align: left; margin-top: 20px; }
                    input, select, textarea { display: block; width: 100%; margin-bottom: 10px; padding: 8px; }
                    button { padding: 10px 20px; background-color: blue; color: white; border: none; cursor: pointer; }
                </style>
            </head>
            <body>
                <h2>Remplissez ce questionnaire</h2>
                <form action="/submit" method="POST">
                    <label for="name">Nom:</label>
                    <input type="text" id="name" name="name" required>
                    
                    <label for="groupe">Groupe:</label>
                    <input type="text" id="groupe" name="groupe" required>
                    
                    <label for="date">Date:</label>
                    <input type="date" id="date" name="date" required>
                    
                    <label for="machine">Référence Machine:</label>
                    <select id="machine" name="machine" required>
                        <option value="">-- Choisissez une machine --</option>
                        <option>DX200_1</option><option>DX200_2</option><option>DX200_3</option><option>DX200_4</option>
                        <option>DX200_5</option><option>DX200_6</option><option>DX200_EXP</option>
                        <option>VX8_1</option><option>VX8_2</option><option>VX8_3</option><option>VX8_4</option>
                        <option>VX8_5</option><option>VX8_6</option><option>VX8_7</option><option>VX8_8</option>
                        <option>VX8_EXP</option><option>AX200_1</option><option>AX200_2</option>
                        <option>AX200_MY</option><option>AX300_MSY</option>
                        <option>UMILL5_15</option><option>UMILL5_18</option>
                    </select>
                    
                    <label>Niveau d'huile:</label>
                    <input type="radio" name="huile" value="Haut"> Haut
                    <input type="radio" name="huile" value="Moyen"> Moyen
                    <input type="radio" name="huile" value="Bas"> Bas
                    
                    <label>Niveau lubrifiant:</label>
                    <input type="radio" name="lubrifiant" value="Haut"> Haut
                    <input type="radio" name="lubrifiant" value="Moyen"> Moyen
                    <input type="radio" name="lubrifiant" value="Bas"> Bas
                    
                    <label for="temp">Température du bloc froid (°C):</label>
                    <input type="number" id="temp" name="temp" step="0.1" required>
                    
                    <label>Niveau bac de copeaux:</label>
                    <input type="radio" name="copeaux" value="Haut"> Haut
                    <input type="radio" name="copeaux" value="Moyen"> Moyen
                    <input type="radio" name="copeaux" value="Bas"> Bas
                    
                    <label>Tiroir haut desserte:</label>
                    <input type="radio" name="tiroir" value="Complet"> Complet
                    <input type="radio" name="tiroir" value="Manquant"> Manquant
                    <input type="radio" name="tiroir" value="Vide"> Vide
                    
                    <label>Propreté zone usinage:</label>
                    <input type="radio" name="usinage" value="Propre"> Propre
                    <input type="radio" name="usinage" value="Moyen"> Moyen
                    <input type="radio" name="usinage" value="Non nettoyé"> Non nettoyé
                    
                    <label>Propreté machine:</label>
                    <input type="radio" name="machine_proprete" value="Propre"> Propre
                    <input type="radio" name="machine_proprete" value="Moyen"> Moyen
                    <input type="radio" name="machine_proprete" value="Non nettoyé"> Non nettoyé
                    
                    <label for="observation">Observation (optionnel):</label>
                    <textarea id="observation" name="observation"></textarea>
                    
                    <button type="submit">Envoyer</button>
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
