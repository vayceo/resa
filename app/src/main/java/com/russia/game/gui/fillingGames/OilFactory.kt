package com.russia.game.gui.fillingGames

import com.russia.game.core.Samp.Companion.activity
import com.russia.game.databinding.OilfactoryBinding
import com.russia.game.gui.NativeGui

class OilFactory
    : NativeGui<OilfactoryBinding>(OilfactoryBinding::class)
{

    private external fun nativeSendExit(ok: Boolean)

    init {
        activity.runOnUiThread {

            ParentClass(
                binding.waterProgress,
                binding.oilProgress,

                binding.waterButton,
                binding.oilButton,

                binding.waterProgressText,
                binding.oilProgressText,

                object : FillingGameListener {
                    override fun onEnded() {
                        destroy()

                        nativeSendExit(true)
                    }
                }
            )

            binding.exitButton.setOnClickListener {
                destroy()
                nativeSendExit(false)
            }
        }
    }

    override fun receivePacket(actionId: Int, data: String) {
        TODO("Not yet implemented")
    }

}