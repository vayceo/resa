package com.russia.game.gui.dialogs;

import android.app.Activity;
import android.content.Context;
import android.text.InputType;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.TextView;

import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.russia.game.R;
import com.russia.game.gui.util.Utils;
import com.nvidia.devtech.NvEventQueueActivity;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;


public class Dialog {
    static final int DIALOG_LEFT_BTN_ID     = 1;
    static final int DIALOG_RIGHT_BTN_ID    = 0;

    static final int DIALOG_STYLE_MSGBOX            = 0;
    static final int DIALOG_STYLE_INPUT             = 1;
    static final int DIALOG_STYLE_LIST              = 2;
    static final int DIALOG_STYLE_PASSWORD          = 3;
    static final int DIALOG_STYLE_TABLIST           = 4;
    static final int DIALOG_STYLE_TABLIST_HEADER    = 5;
    static final int DIALOG_STYLE_INPUT_NUMBER      = 6;


    private final TextView mCaption;
    private final TextView mContent;
    private int mCurrentDialogId = -1;
    private int mCurrentDialogTypeId = -1;
    private String mCurrentInputText = "";
    private int mCurrentListItem = -1;
    private boolean mInputPasswordStyle = false;
    private final RecyclerView mCustomRecyclerView;
    private final ArrayList<TextView> mHeadersList;
    private final EditText mInput;
    private final ConstraintLayout mInputLayout;
    private final ConstraintLayout mLeftBtn;
    private final ConstraintLayout mListLayout;
    private final ConstraintLayout mMainLayout;
    private final ScrollView mMsgBoxLayout;
    private final ConstraintLayout mRightBtn;
    private ArrayList<String> mRowsList;
    private Activity activity;
    boolean old_casino_layout_state;

    native void init();
    native void sendResponse(int button, int id, int item, byte[] str);

    public Dialog(Activity aactivity) {
        init();

        activity = aactivity;
        this.mMainLayout = activity.findViewById(R.id.sd_dialog_main);
        this.mCaption = (TextView) activity.findViewById(R.id.sd_dialog_caption);
        this.mContent = (TextView) activity.findViewById(R.id.sd_dialog_text);
        ConstraintLayout findViewById1 = activity.findViewById(R.id.sd_button_positive);
        this.mLeftBtn = findViewById1;
        ConstraintLayout findViewById2 = activity.findViewById(R.id.sd_button_negative);
        this.mRightBtn = findViewById2;
        this.mInputLayout = activity.findViewById(R.id.sd_dialog_input_layout);
        this.mListLayout = activity.findViewById(R.id.sd_dialog_list_layout);
        this.mMsgBoxLayout = (ScrollView) activity.findViewById(R.id.sd_dialog_text_layout);
        this.mInput = activity.findViewById(R.id.sd_dialog_input);
        this.mCustomRecyclerView = activity.findViewById(R.id.sd_dialog_list_recycler);
        findViewById1.setOnClickListener(view -> sendDialogResponse(1));
        findViewById2.setOnClickListener(view -> sendDialogResponse(0));
        this.mRowsList = new ArrayList<>();
        this.mHeadersList = new ArrayList<>();

        ConstraintLayout mHeadersLayout = activity.findViewById(R.id.sd_dialog_tablist_row);

        for (int i = 0; i < mHeadersLayout.getChildCount(); i++) {
            this.mHeadersList.add((TextView) mHeadersLayout.getChildAt(i));
        }

        Utils.HideLayout(this.mMainLayout, false);
    }

    public void show(int dialogId, int dialogTypeId, String caption, String content, String leftBtnText, String rightBtnText) {
        switch (dialogTypeId)
        {
            case DIALOG_STYLE_INPUT:{
                mInput.setInputType(InputType.TYPE_CLASS_TEXT);
                break;
            }
            case DIALOG_STYLE_PASSWORD:{
                mInput.setInputType(InputType.TYPE_TEXT_VARIATION_PASSWORD);
                break;
            }
            case DIALOG_STYLE_INPUT_NUMBER:{
                mInput.setInputType(InputType.TYPE_CLASS_NUMBER);
                break;
            }
        }
        activity.runOnUiThread(() -> {
            clearDialogData();
//            final ConstraintLayout casino_dice_main_layout =  activity.findViewById(R.id.casino_dice_main_layout);
//            if (casino_dice_main_layout != null && casino_dice_main_layout.getVisibility() == View.VISIBLE ) {
//                old_casino_layout_state = true;
//                casino_dice_main_layout.setVisibility(View.GONE);
//            }

            this.mCurrentDialogId = dialogId;
            this.mCurrentDialogTypeId = dialogTypeId;
            this.mInputPasswordStyle = false;
            if (dialogTypeId == 0) {
                this.mInputLayout.setVisibility(View.GONE);
                this.mListLayout.setVisibility(View.GONE);
                this.mMsgBoxLayout.setVisibility(View.VISIBLE);
            } else if (dialogTypeId == DIALOG_STYLE_INPUT || dialogTypeId == DIALOG_STYLE_PASSWORD ||
                    dialogTypeId == DIALOG_STYLE_INPUT_NUMBER) {
                this.mInputLayout.setVisibility(View.VISIBLE); // РІС‹РїРѕР»РЅСЏРµС‚СЃСЏ РёРЅРїСѓС‚
                this.mMsgBoxLayout.setVisibility(View.VISIBLE);
                this.mListLayout.setVisibility(View.GONE);
            } else {
                this.mInputLayout.setVisibility(View.GONE);
                this.mMsgBoxLayout.setVisibility(View.GONE); // LIST, TABLIST, TABLIST_HEADER
                this.mListLayout.setVisibility(View.VISIBLE);
                loadTabList(content);
                //ArrayList<String> fixFieldsForDialog = Utils.fixFieldsForDialog(this.mRowsList);
               // this.mRowsList = fixFieldsForDialog;
                DialogAdapter adapter = new DialogAdapter(this.mRowsList, this.mHeadersList);
                adapter.setOnClickListener((i, str) -> {
                    this.mCurrentListItem = i;
                    this.mCurrentInputText = str;
                });
                adapter.setOnDoubleClickListener(() -> sendDialogResponse(1));
                this.mCustomRecyclerView.setLayoutManager(new LinearLayoutManager(activity));
                this.mCustomRecyclerView.setAdapter(adapter);

                mMainLayout.post(() ->{

                    int width = mCaption.getWidth();
                    if(mCustomRecyclerView.getMinimumWidth() < width)
                    {
                        mCustomRecyclerView.setMinimumWidth(width);
                    }
                    if (dialogTypeId != 2) {
                        adapter.updateSizes();
                    }
                    mCustomRecyclerView.requestLayout();
                } );

            }
            this.mCaption.setText(Utils.transfromColors(caption));
            this.mContent.setText(Utils.transfromColors(content));
            ((TextView) this.mLeftBtn.getChildAt(0)).setText(Utils.transfromColors(leftBtnText));
            ((TextView) this.mRightBtn.getChildAt(0)).setText(Utils.transfromColors(rightBtnText));
            if (rightBtnText.equals("")) {
                this.mRightBtn.setVisibility(View.GONE);
            } else {
                this.mRightBtn.setVisibility(View.VISIBLE);
            }

            Utils.ShowLayout(this.mMainLayout, false);
        });

    }

    public void hideWithoutReset() {
        Utils.HideLayout(this.mMainLayout, false);
    }

    public void showWithOldContent() {
        Utils.ShowLayout(this.mMainLayout, false);
    }

    public void sendDialogResponse(int btnId)
    {
//        if(old_casino_layout_state)
//        {
//            old_casino_layout_state = false;
//            final ConstraintLayout casino_dice_main_layout =  activity.findViewById(R.id.casino_dice_main_layout);
//            if (casino_dice_main_layout != null )
//                casino_dice_main_layout.setVisibility(View.VISIBLE);
//        }
        if (    mCurrentDialogTypeId == DIALOG_STYLE_INPUT ||
                mCurrentDialogTypeId == DIALOG_STYLE_PASSWORD||
                mCurrentDialogTypeId == DIALOG_STYLE_INPUT_NUMBER)
        {
            this.mCurrentInputText = this.mInput.getText().toString();
        }
        else if(mCurrentDialogTypeId == DIALOG_STYLE_MSGBOX) {
            this.mCurrentInputText = "";
        }
        try {

            ((InputMethodManager) activity.getSystemService(Context.INPUT_METHOD_SERVICE)).hideSoftInputFromWindow(this.mInput.getWindowToken(), 0);

            sendResponse(btnId, mCurrentDialogId, mCurrentListItem, mCurrentInputText.getBytes("windows-1251"));


            Utils.HideLayout(this.mMainLayout, false);
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }

//            Charset w1251 = Charset.forName("Windows-1251");
//            Charset utf8 = Charset.forName("UTF-8");
//
//            String string = new String (buffer, w1251);

        // String utf8String= new String(this.mCurrentInputText.getBytes("windows-1251"), "UTF-8");



    }
    public void hide() {
        activity.runOnUiThread(()-> {
            Utils.HideLayout(this.mMainLayout, false);
        });
    }
    private void loadTabList(String content) {
        String[] strings = content.split("\n");
        for (int i = 0; i < strings.length; i++) {
            if (this.mCurrentDialogTypeId == DIALOG_STYLE_TABLIST_HEADER && i == 0) {
                String[] headers = strings[i].split("\t");
                for (int j = 0; j < headers.length; j++) {
                    this.mHeadersList.get(j).setText(Utils.transfromColors(headers[j]));
                   // this.mHeadersList.get(j).setWidth(headers[j].length()*25);
                    this.mHeadersList.get(j).setVisibility(View.VISIBLE);
                }
            } else {
                this.mRowsList.add(strings[i]);
            }
        }
    }

    private void clearDialogData() {
        mCustomRecyclerView.setMinimumWidth(300);
        this.mInput.setText("");
        this.mCurrentDialogId = -1;
        this.mCurrentDialogTypeId = -1;
        this.mCurrentListItem = -1;
        this.mRowsList.clear();
        this.mCustomRecyclerView.setAdapter(null);

        for (int i = 0; i < this.mHeadersList.size(); i++) {
            this.mHeadersList.get(i).setText("");
            this.mHeadersList.get(i).setVisibility(View.GONE);
        }
    }

//    public void onHeightChanged(int height) {
//        FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) this.mMainLayout.getLayoutParams();
//        params.setMargins(0, 0, 0, height);
//        this.mMainLayout.setLayoutParams(params);
//    }
}
