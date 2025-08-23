package com.russia.launcher.web;

import android.content.Context;
import android.webkit.JavascriptInterface;

import com.russia.launcher.ui.dialogs.ReCaptchaDialog;

public class ReCaptchaWebInterface {
    private Context mContext;
    private ReCaptchaDialog reCaptchaDialog;
    private String data;

    public ReCaptchaWebInterface(Context ctx, ReCaptchaDialog reCaptchaDialog){
        this.mContext = ctx;
        this.reCaptchaDialog = reCaptchaDialog;
    }


    @JavascriptInterface
    public void recaptchaPass(String data) {
        this.data = data;
        reCaptchaDialog.performCaptchaPassAction(data);
    }
}
