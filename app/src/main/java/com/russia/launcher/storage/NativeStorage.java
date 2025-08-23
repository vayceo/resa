package com.russia.launcher.storage;

import static com.russia.launcher.config.Config.NATIVE_SETTINGS_FILE_PATH;

import android.content.Context;
import android.widget.Toast;

import com.russia.launcher.domain.messages.ErrorMessage;

import org.ini4j.InvalidFileFormatException;
import org.ini4j.Wini;

import java.io.File;
import java.io.IOException;

public class NativeStorage {

    private static final String CLIENT_SECTION_NAME = "client";



    public static void addClientProperty(String propertyName, String value, Context context) {
        try {
            File f = new File(context.getExternalFilesDir(null) + NATIVE_SETTINGS_FILE_PATH);

            if (!f.exists()) {
                return;
            }

            Wini w = new Wini(new File(context.getExternalFilesDir(null) + NATIVE_SETTINGS_FILE_PATH));
            w.put(CLIENT_SECTION_NAME, propertyName, value);
            w.store();
        } catch (InvalidFileFormatException e) {
            throw new RuntimeException(e);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static String getClientProperty(String property, Context context) {

        String value = null;

        try {
            Wini w = new Wini(new File(context.getExternalFilesDir(null) + NATIVE_SETTINGS_FILE_PATH));
            value = w.get(CLIENT_SECTION_NAME, property);
            w.store();
        } catch (IOException ignored) {

        }

        return value;
    }

    private static void showMessage(String message, Context context) {
        Toast.makeText(context, message, Toast.LENGTH_SHORT)
                .show();
    }
}
