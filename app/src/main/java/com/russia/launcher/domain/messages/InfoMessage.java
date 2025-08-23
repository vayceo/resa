package com.russia.launcher.domain.messages;

import lombok.Getter;
import lombok.RequiredArgsConstructor;

@Getter
@RequiredArgsConstructor
public enum InfoMessage {
    CONNECTION_TO_LAST_SERVER("Подключение к последнему выбранному серверу!"),
    CONNECTION_TO_SELECT_SERVER("Подключение к серверу: %s!"),
    SERVER_SELECTED("Сервер выбран! Для начала игры нажмите жёлтую кнопку"),
    INSTALL_GAME_FIRST("Сначала установите игру"),
    GAME_FILES_VALID("Содержимое файлов валидно!"),
    SETTINGS_ALREADY_DEFAULT("Настройки по умолчанию уже установлены"),
    DOWNLOAD_SUCCESS_INPUT_YOUR_NICKNAME("Игра успешно установлена, введите свой никнейм"),
    DOWNLOAD_SUCCESS_SELECT_SERVER("Игра успешно установлена, выберите сервер"),
    REINSTALL_GAME_QUESTION("Переустановить игру?"),
    RESET_SETTINGS_QUESTION("Сбросить настройки игры?"),
    APPROVE_INSTALL("Подтвердите установку");

    private final String text;

    public static String createConnectToSelectServerMessage(String serverName) {
        return String.format(
                CONNECTION_TO_SELECT_SERVER.getText(),
                serverName
        );
    }
}
