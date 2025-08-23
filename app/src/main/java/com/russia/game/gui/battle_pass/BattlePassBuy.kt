package com.russia.game.gui.battle_pass

import com.russia.game.R
import com.russia.game.core.Samp.Companion.activity
import com.russia.game.databinding.BattlepassBuyBinding
import com.russia.game.gui.NativeGui
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale
import java.util.TimeZone

class BattlePassBuy : NativeGui<BattlepassBuyBinding>(BattlepassBuyBinding::class) {
    enum class eClickType {
        CLICK_BUY_1,
        CLICK_BUY_2,
        CLICK_BUY_3,
        CLICK_PREVIEW,
    }
    private external fun nativeOnClick(clickType: Int)
    private external fun nativeOnClosed()

    init {
        activity.runOnUiThread {
            binding.exitButton.setOnClickListener {
                destroy()
            }

            binding.buyType1.setOnClickListener {
                nativeOnClick(eClickType.CLICK_BUY_1.ordinal)
            }

            binding.buyType2.setOnClickListener {
                nativeOnClick(eClickType.CLICK_BUY_2.ordinal)
            }

            binding.buyType3.setOnClickListener {
                nativeOnClick(eClickType.CLICK_BUY_3.ordinal)
            }

            binding.previewButton.setOnClickListener {
                nativeOnClick(eClickType.CLICK_PREVIEW.ordinal)
            }
        }
    }

    private fun show(endTime: Long) {
        activity.runOnUiThread {
            val date = Date(endTime * 1000L)
            val sdf = SimpleDateFormat("d MMMM yyyy", Locale("ru", "RU"))
            sdf.timeZone = TimeZone.getTimeZone("Europe/Moscow")

            val formattedDate = sdf.format(date)

            binding.endDate.text = "Дата окончания: $formattedDate"
        }
    }

    override fun destroy() {
        super.destroy()
        nativeOnClosed()
    }

    override fun receivePacket(actionId: Int, data: String) {
        TODO("Not yet implemented")
    }
}