//
// Created by avivx on 08/01/2021.
//

#include "CLI.h"

void CLI::start() {
    getCommands();
    printMenu();
    string option = dio->read();
    int optionNum = stoi(option);
    while (optionNum != 6) {
        commands[optionNum - 1]->execute();
        printMenu();
        option = dio->read();
        optionNum = stoi(option);

    }
    commands[5]->execute();

}

CLI::CLI(DefaultIO *dio) {
    this->dio = dio;
}

void CLI::printMenu() {
    dio->write("Welcome to the Anomaly Detection Server.\n");
    dio->write("Please choose an option:\n");
    for (int i = 0; i < 6; i++) {
        dio->write(commands[i]->getDescription());
        dio->write("\n");
    }
}

void CLI::getCommands() {
    commands = new Command*[6];
    auto *cm = new commandsManagement();
    auto *uploadTS = new UploadTimeSeries(dio, cm);
    auto *settings = new Settings(dio, cm);
    auto *ad = new AnomalyDetection(dio, cm);
    auto *res = new Results(dio, cm);
    auto *analyze = new AnalyzeResults(dio, cm);
    auto *exit = new Exit(dio, cm, uploadTS, settings, ad, res, analyze);
    commands[0] = uploadTS;
    commands[1] = settings;
    commands[2] = ad;
    commands[3] = res;
    commands[4] = analyze;
    commands[5] = exit;

}

CLI::~CLI() {
    delete [] commands;
}







