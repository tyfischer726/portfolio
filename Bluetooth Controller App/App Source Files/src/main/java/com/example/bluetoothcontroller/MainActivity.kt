package com.example.bluetoothcontroller

import android.app.Activity
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothManager
import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.widget.Adapter
import android.widget.AdapterView
import android.widget.ArrayAdapter
import android.widget.Toast
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_main.*
import kotlin.math.log


class MainActivity : AppCompatActivity() {

    lateinit var bAdapter:BluetoothAdapter
    lateinit var m_pairedDevices: Set<BluetoothDevice>
    val REQUEST_ENABLE_BLUETOOTH = 1

    companion object {
        val EXTRA_ADDRESS: String = "Device_address"
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val bluetoothManager = this.getSystemService(Context.BLUETOOTH_SERVICE) as BluetoothManager
        bAdapter = bluetoothManager.getAdapter()

        //check if device is bluetooth compatible
        if (bAdapter == null){
            statusTv.text = "This device does not support bluetooth."
        }
        //check if bluetooth is on
        if (bAdapter.isEnabled) {
            statusTv.text = "Bluetooth is on."
        } else {
            statusTv.text = "Bluetooth is off. Check device settings."
        }

        //turn on bluetooth
        turnOnBtn.setOnClickListener {
            if (bAdapter.isEnabled) {
                Toast.makeText(this, "Already on.", Toast.LENGTH_SHORT).show()
            } else {
                var m_intent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
                activityResultLauncher.launch(m_intent)
            }
        }

        //turn off bluetooth
        turnOffBtn.setOnClickListener {
            if (!bAdapter.isEnabled) {
                Toast.makeText(this, "Already off.", Toast.LENGTH_SHORT).show()
            } else {
                bAdapter.disable()
                Toast.makeText(this, "Bluetooth is off.", Toast.LENGTH_SHORT).show()
                statusTv.text = "Bluetooth is off. Check device settings."
            }
        }

        refresh_button.setOnClickListener { pairedDeviceList() }
    }

    var activityResultLauncher = registerForActivityResult(
        ActivityResultContracts.StartActivityForResult()) { result ->
        if (result.resultCode == Activity.RESULT_OK) {
            Toast.makeText(this, "Bluetooth is on.", Toast.LENGTH_SHORT).show()
            statusTv.text = "Bluetooth is on."
        }
        else {
            Toast.makeText(this, "Could not turn on bluetooth.", Toast.LENGTH_SHORT).show()
        }
    }

    private fun pairedDeviceList() {
        m_pairedDevices = bAdapter.bondedDevices
        val list: ArrayList<BluetoothDevice> = ArrayList()
        val name_list: ArrayList<String> = ArrayList()

        if (!m_pairedDevices.isEmpty()) {
            for (device: BluetoothDevice in m_pairedDevices) {
                list.add(device)
                name_list.add(device.name)
                Log.i("device", ""+device)
            }
        } else {
            Toast.makeText(this, "No devices found.", Toast.LENGTH_SHORT).show()
        }

        val adapter = ArrayAdapter(this,android.R.layout.simple_list_item_1, name_list)
        select_device_list.adapter = adapter
        select_device_list.onItemClickListener = AdapterView.OnItemClickListener { _, _, position, _ ->
            val device: BluetoothDevice = list[position]
            val address: String = device.address

            val intent = Intent(this, ControlActivity::class.java)
            intent.putExtra(EXTRA_ADDRESS, address)
            startActivity(intent)
        }
    }
}