#include <crow.h>
#include <nlohmann/json.hpp>
#include <zmq.hpp>
#include <thread>
#include <iostream>
#include <map>

int main() {
    crow::SimpleApp app;
    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcp://*:5555"); // Port ZeroMQ
    std::cout << "✅ ZeroMQ Publisher lié au port 5555" << std::endl;
    
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
                    body { 
                        font-family: 'Arial', sans-serif; 
                        text-align: center; 
                        background-color: #f4f4f4;
                        color: #333;
                    }
                    form { 
                        display: inline-block; 
                        text-align: left; 
                        margin-top: 20px; 
                        background: white;
                        padding: 20px;
                        border-radius: 10px;
                        box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
                        width: 350px;
                    }
                    input, select, textarea { 
                        display: block; 
                        width: 94%; 
                        margin-bottom: 20px; 
                        padding: 10px; 
                        border: 1px solid #ccc; 
                        border-radius: 10px;
                    }
                    .button-group {
                        margin-bottom: 20px;
                    }
                    .button-group label { 
                        display: block;
                        padding: 12px;
                        border-radius: 10px;
                        text-align: center;
                        cursor: pointer;
                        transition: background 0.3s;
                        background: #e0e0e0;
                        margin-bottom: 5px;
                    }
                    .button-group input {
                        display: none;
                    }
                    .button-group input:checked + label {
                        background: #5fa5f0f3;
                        color: white;
                    }
                    button { 
                        padding: 10px 20px; 
                        background-color: #007BFF; 
                        color: white; 
                        border: none; 
                        cursor: pointer; 
                        border-radius: 5px;
                        transition: background 0.3s;
                    }
                    button:hover {
                        background-color: #0056b3;
                    }
                    .title-container {
                        display: inline-block;
                        padding: 20px 21px;
                        border-radius: 10px;
                        background-color: white;
                        color: white;
                        box-shadow: 0 0 20px rgba(0, 0, 0, 0.1);
                        margin-top: 20px;
                    }
                    h1 {
                        font-size: 30px;
                        color: rgba(71, 69, 69, 0.842);
                        margin: 5px 0;
                    }
                    h2 {
                        font-size: 20px;
                        color: rgba(71, 69, 69, 0.842)
                    }
                </style>
            </head>
            <body>
                <div class="title-container">
                    <h1>Etat du centre d'usinage</h1>
                    <h2>Remplissez ce questionnaire</h2>
                    <h2>avant d'utiliser la machine</h2>
                </div>

                <form action="/submit" method="POST">
                    <label for="name">Nom :</label>
                    <input type="text" id="name" name="name" required>

                    <label for="group">Groupe :</label>
                    <input type="text" id="group" name="group" required>

                    <label for="date">Date :</label>
                    <input type="date" id="date" name="date" required value="" readonly>
                    <script>
                        document.getElementById('date').valueAsDate = new Date();
                    </script>

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

                    <label>Niveau d'huile :</label>
                    <div class="button-group">
                        <input type="radio" id="huileHaut" name="huile" value="Haut"><label for="huileHaut">Haut</label>
                        <input type="radio" id="huileMoyen" name="huile" value="Moyen"><label for="huileMoyen">Moyen</label>
                        <input type="radio" id="huileBas" name="huile" value="Bas"><label for="huileBas">Bas</label>
                    </div>
                    
                    <label>Niveau lubrifiant :</label>
                    <div class="button-group">
                        <input type="radio" id="lubHaut" name="lubrifiant" value="Haut"><label for="lubHaut">Haut</label>
                        <input type="radio" id="lubMoyen" name="lubrifiant" value="Moyen"><label for="lubMoyen">Moyen</label>
                        <input type="radio" id="lubBas" name="lubrifiant" value="Bas"><label for="lubBas">Bas</label>
                    </div>
                    
                    <label for="temp">Température du bloc froid :</label>
                    <input type="number" id="temp" name="temp" step="0.1" required>
                    
                    <label>Remplissage bac copeaux :</label>
                    <div class="button-group">
                        <input type="radio" id="copeauxHaut" name="copeaux" value="Haut"><label for="copeauxHaut">Haut</label>
                        <input type="radio" id="copeauxMoyen" name="copeaux" value="Moyen"><label for="copeauxMoyen">Moyen</label>
                        <input type="radio" id="copeauxBas" name="copeaux" value="Bas"><label for="copeauxBas">Bas</label>
                    </div>

                    <label>Tiroir haut desserte :</label>
                    <div class="button-group">
                        <input type="radio" id="tiroirComplet" name="tiroir" value="Complet"><label for="tiroirComplet">Complet</label>
                        <input type="radio" id="tiroirManquant" name="tiroir" value="Manquant"><label for="tiroirManquant">Manquant</label>
                        <input type="radio" id="tiroirVide" name="tiroir" value="Vide"><label for="tiroirVide">Vide</label>
                    </div>

                    <label>Propreté zone d'usinage :</label>
                    <div class="button-group">
                        <input type="radio" id="prop_zonePropre" name="prop_zone" value="Propre"><label for="prop_zonePropre">Propre</label>
                        <input type="radio" id="prop_zoneMoyen" name="prop_zone" value="Moyen"><label for="prop_zoneMoyen">Moyen</label>
                        <input type="radio" id="prop_zoneNonNettoyé" name="prop_zone" value="NonNettoyé"><label for="prop_zoneNonNettoyé">Non Nettoyé</label>
                    </div>

                    <label>Propreté machine :</label>
                    <div class="button-group">
                        <input type="radio" id="prop_machPropre" name="prop_mach" value="Propre"><label for="prop_machPropre">Propre</label>
                        <input type="radio" id="prop_machMoyen" name="prop_mach" value="Moyen"><label for="prop_machMoyen">Moyen</label>
                        <input type="radio" id="prop_machNonNettoyé" name="prop_mach" value="NonNettoyé"><label for="prop_machNonNettoyé">Non Nettoyé</label>
                    </div>

                    <label for="observation">Observation (optionnel) :</label>
                    <textarea id="observation" name="observation"></textarea>
                    
                    <button type="submit">Soumettre</button>
                </form>
                <script>
                    document.querySelector("form").addEventListener("submit", function(event) {
                        event.preventDefault(); // Prevent normal form submission

                        let formData = new FormData(this);
                        let jsonObject = {};
    
                        formData.forEach((value, key) => {
                            jsonObject[key] = value;
                        });

                        fetch("/submit", {
                            method: "POST",
                            headers: {
                                "Content-Type": "application/json"
                            },
                            body: JSON.stringify(jsonObject)
                        })
                        .then(response => response.json())
                        .then(data => {
                            console.log("Server Response:", data);
                            alert("Form submitted successfully!");
                        })
                        .catch(error => console.error("Error:", error));
                    });
                </script>
            </body>
            </html>
        )";
    });
    
    // Route to handle form submission (POST request)
    CROW_ROUTE(app, "/submit").methods("POST"_method)
    ([&publisher](const crow::request& req) {
        try {
            // Parse JSON from the request body
            auto json_data = nlohmann::json::parse(req.body);
        
            // Store all fields in a map
            std::map<std::string, std::string> answers;
            for (auto& el : json_data.items()) {
                answers[el.key()] = el.value();
            }

            // Log received responses
            CROW_LOG_INFO << "Nouvelle réponse reçue:";
            for (const auto& [key, value] : answers) {
                CROW_LOG_INFO << key << ": " << value;
            }

            // Convertir la map en JSON
            nlohmann::json json_message = answers;
            std::string message = json_message.dump();

            // Envoi des données au broker via ZeroMQ
            zmq::message_t zmqMessage(message.begin(), message.end());
            std::cout << "📤 Envoi du message ZMQ: " << message << std::endl;
            publisher.send(zmqMessage, zmq::send_flags::none);
            
            std::cout << "Données envoyées : " << message << std::endl;
            return crow::response{json_data.dump(4)}; // Send back the received data as JSON response
        
        } catch (const std::exception& e) {
            return crow::response(400, std::string("Invalid JSON format: ") + e.what());
        }
    });

    app.bindaddr("0.0.0.0").port(5000).multithreaded().run();
}
