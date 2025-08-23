package com.russia.game.gui

import com.russia.game.EntitySnaps
import com.russia.game.R
import com.russia.game.core.Samp
import com.russia.game.core.Samp.Companion.activity
import com.russia.game.databinding.TechnicalInspectionBinding
import com.russia.data.vehicles.Vehicles

class TechIspect : NativeGui<TechnicalInspectionBinding>(TechnicalInspectionBinding::class) {
    private external fun sendClick()
    private external fun exit()

    init {
        activity.runOnUiThread {
            binding.exitButton.setOnClickListener {
                destroy()
            }

            binding.buyButton.setOnClickListener {
                sendClick()

                destroy()
            }

        }
    }

    fun show(
        name: String,
        gen: Int,
        candl: Int,
        brake: Int,
        starter: Int,
        nozzles: Int,
        price: Int,
        carid: Int
    ) {
        activity.runOnUiThread {

            EntitySnaps.loadEntitySnapToImageView(Vehicles.getSnap(carid), binding.vehImage)
            binding.vehNameText.text = name

            binding.generatorPercent.text = String.format("%d %%", 100 - gen)
            binding.generatoProgress.progress = (100 - gen).toFloat()

            binding.candlesPercent.text = String.format("%d %%", 100 - candl)
            binding.candlesProgress.progress = (100 - candl).toFloat()

            binding.brakePercent.text = String.format("%d %%", 100 - brake)
            binding.brakeProgress.progress = (100 - brake).toFloat()

            binding.starterPercent.text = String.format("%d %%", 100 - starter)
            binding.starterProgress.progress = (100 - starter).toFloat()

            binding.nozzlesPercent.text = String.format("%d %%", 100 - nozzles)
            binding.nozzlesProgress.progress = (100 - nozzles).toFloat()

            binding.priceText.text =
                String.format("Госпошлина %s рублей", Samp.formatter.format(price.toLong()))
        }
    }

    override fun destroy() {
        super.destroy()
        exit()
    }

    override fun receivePacket(actionId: Int, data: String) {
        TODO("Not yet implemented")
    }
}