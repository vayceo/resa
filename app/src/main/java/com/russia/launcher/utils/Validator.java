package com.russia.launcher.utils;

import android.app.Activity;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import com.russia.game.R;
import com.russia.launcher.domain.messages.ErrorMessage;
import com.russia.launcher.async.dto.response.Servers;

import org.apache.commons.lang3.StringUtils;

public class Validator {

    private static final int NICKNAME_MAX_LENGTH = 20;
    private static final int EMAIL_MAX_LENGTH = 100;
    private static final int PASSWORD_MAX_LENGTH = 16;
    private static final int PASSWORD_MIN_LENGTH = 6;

    public static boolean isValidNickname(String nickname, Activity activity) {

        if (StringUtils.isBlank(nickname)) {
            showMessage(ErrorMessage.NICKNAME_IS_EMPTY.getText(), activity);
            return false;
        }

        if (nickname.matches(InputPatterns.STRING_WITH_DIGITS_PATTERN)) {
            showMessage(ErrorMessage.NICKNAME_CONTAINS_DIGITS.getText(), activity);
            return false;
        }

        if (nickname.length() > NICKNAME_MAX_LENGTH) {
            showMessage(ErrorMessage.NICKNAME_IS_TOO_LONG.getText(), activity);
            return false;
        }

        if (!nickname.matches(InputPatterns.NICKNAME_PATTERN)) {
            showMessage(ErrorMessage.NICKNAME_DOES_NOT_MATCH_PATTERN.getText(), activity);
            return false;
        }

        return true;
    }

    public static boolean isValidEmail(String email, Activity activity) {

        if (StringUtils.isBlank(email)) {
            showMessage(ErrorMessage.EMAIL_IS_EMPTY.getText(), activity);
            return false;
        }

        if (email.length() > EMAIL_MAX_LENGTH) {
            showMessage(ErrorMessage.EMAIL_IS_TOO_LONG.getText(), activity);
            return false;
        }

        if (!email.matches(InputPatterns.EMAIL_PATTERN)) {
            showMessage(ErrorMessage.EMAIL_DOES_NOT_MATCH_PATTERN.getText(), activity);
            return false;
        }

        return true;
    }

    public static boolean isValidDonateSum(String donateSum, Activity activity) {

        if (StringUtils.isBlank(donateSum)) {
            showMessage(ErrorMessage.DONATE_SUM_IS_EMPTY.getText(), activity);
            return false;
        }

        if (!donateSum.matches(InputPatterns.STRING_ONLY_DIGITS_PATTERN)) {
            showMessage(ErrorMessage.DONATE_SUM_CONTAINS_SYMBOLS_OTHER_DIGITS.getText(), activity);
            return false;
        }

        return true;
    }

    public static boolean isValidPassword(String password, Activity activity) {

        if (StringUtils.isBlank(password)) {
            showMessage(ErrorMessage.PASSWORD_IS_EMPTY.getText(), activity);
            return false;
        }

        if (password.length() > PASSWORD_MAX_LENGTH) {
            showMessage(ErrorMessage.PASSWORD_IS_TOO_LONG.getText(), activity);
            return false;
        }

        if (password.length() < PASSWORD_MIN_LENGTH) {
            showMessage(ErrorMessage.PASSWORD_IS_TOO_SHORT.getText(), activity);
            return false;
        }

        return true;
    }

    public static boolean isValidSelectedServer(Servers selectedServer, Activity activity) {

        if (selectedServer == null) {
            showMessage(ErrorMessage.SERVER_NOT_SELECTED.getText(), activity);
            return false;
        }

        return true;
    }

    public static boolean isValidCaptchaToken(String captchaToken, Activity activity) {

        if (StringUtils.isBlank(captchaToken)) {
            showMessage(ErrorMessage.CAPTCHA_NOT_PASSED.getText(), activity);
            return false;
        }

        return true;
    }

    private static void showMessage(String message, Activity activity) {
        LayoutInflater inflater = activity.getLayoutInflater();
        View layout = inflater.inflate(R.layout.toast_error,
                (ViewGroup) activity.findViewById(R.id.toast_layout_root));

        TextView text = (TextView) layout.findViewById(R.id.br_not_text);
        text.setText(message);

        Toast toast = new Toast(activity.getApplicationContext());
        toast.setGravity(Gravity.BOTTOM, 0, 0);
        toast.setDuration(Toast.LENGTH_LONG);
        toast.setView(layout);
        toast.show();
    }
}
