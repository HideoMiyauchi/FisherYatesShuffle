package hideo.miyauchi.fisheryatesshuffle

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import hideo.miyauchi.fisheryatesshuffle.databinding.ActivityMainBinding
import java.nio.ByteBuffer

class MainActivity : AppCompatActivity() {

    private lateinit var activityMainBinding: ActivityMainBinding
    private external fun jniFisherYatesShuffle(
        rawBitmapByteArray:ByteArray, width:Int, height:Int, rowBytes:Int,
        bytesPerPixel:Int, randomSequence:IntArray, divV:Int, divH:Int, reverse:Boolean)

    private val divWidthImage = 10 // width of a cell
    private val divHeightImage = 10 // height of a cell
    private var baseImage: Bitmap? = null
    private var shuffleImage: Bitmap? = null
    private var restoreImage: Bitmap? = null
    private var randomSequence: IntArray? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        activityMainBinding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(activityMainBinding.root)

        // Load JNI library
        System.loadLibrary("FisherYatesShuffle")

        // get bitmap from asset and draw to imageview
        baseImage = BitmapFactory.decodeStream(assets.open("LisaDelGiocondo.jpg"))
        activityMainBinding.imageViewBase.setImageBitmap(baseImage)

        // On Click listener
        activityMainBinding.buttonStart.setOnClickListener {

            if (shuffleImage == null && baseImage != null) {
                // Deep copy bitmap image
                shuffleImage = baseImage!!.copy(baseImage!!.config, true)
                fisherYatesShuffle(shuffleImage!!)
                activityMainBinding.imageViewShuffle.setImageBitmap(shuffleImage)

            } else if (restoreImage == null && shuffleImage != null) {
                // Deep copy bitmap image
                restoreImage = shuffleImage!!.copy(shuffleImage!!.config, true)
                fisherYatesShuffle(restoreImage!!, true)
                activityMainBinding.imageViewRestore.setImageBitmap(restoreImage)
            }
        }
    }

    @Suppress("RemoveRedundantCallsOfConversionMethods")
    private fun fisherYatesShuffle(bitmap: Bitmap, restore: Boolean = false) {

        // logic pre-check
        val bytesPerPixel = 4
        if (bitmap.byteCount != (bitmap.rowBytes * bitmap.height)) {
            return
        }
        if (bitmap.rowBytes != (bitmap.width * bytesPerPixel)) {
            return
        }

        // bitmap ---> ByteArray
        val byteBuffer = ByteBuffer.allocate(bitmap.byteCount)
        bitmap.copyPixelsToBuffer(byteBuffer)
        val rawBitmapByteArray = byteBuffer.array()

        // division number
        val divH: Int = (bitmap.width / divWidthImage).toInt()
        val divV: Int = (bitmap.height / divHeightImage).toInt()

        // shuffled array for Fisher-Yates suffle algorithm
        if (randomSequence == null) {
            randomSequence = IntArray(divH * divV) { it }
            randomSequence?.shuffle()
        }

        // call native implementation
        jniFisherYatesShuffle(rawBitmapByteArray, bitmap.width, bitmap.height, bitmap.rowBytes,
            bytesPerPixel, randomSequence!!, divV, divH, restore)

        // ByteArray ---> bitmap
        bitmap.copyPixelsFromBuffer(ByteBuffer.wrap(rawBitmapByteArray))
    }
}