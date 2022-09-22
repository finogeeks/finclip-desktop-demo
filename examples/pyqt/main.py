from PyQt5.QtWidgets import (QApplication, QComboBox, QDialog,
                             QDialogButtonBox, QFormLayout, QGridLayout, QGroupBox, QHBoxLayout,
                             QLabel, QLineEdit, QMenu, QMenuBar, QPushButton, QSpinBox, QTextEdit,
                             QVBoxLayout)
import finclip
import sys
import json


class PythonCallback(finclip.Callback):

    # Define Python class 'constructor'
    def __init__(self):
        # Call C++ base class constructor
        finclip.Callback.__init__(self)

    def Run(self, event, param):
        print(event)
        print(param)
        return "{}"


appid = "60e3c059949a5300014d0c07"

callback = PythonCallback().__disown__()


def start_finclip():
    factory = finclip.finclip_get_packer_factory()
    packer = finclip.finclip_packer_factory_get_config_packer(factory)
    config = finclip.finclip_config_packer_new_config(packer)
    finclip.finclip_config_set_app_store(config, 1)
    finclip.finclip_config_set_app_key(config,
                                       "")
    finclip.finclip_config_set_secret(config, "")
    finclip.finclip_config_set_domain(
        config, "")
    if sys.platform == 'win32':
        finclip.finclip_config_set_value(
            config, finclip.FINCLIP_CONFIG_EXE_PATH, "")
    finclip.finclip_config_packer_add_config(packer, config)
    # callback.thisown = 0
    finclip.finclip_register_callback_cpp(
        packer, finclip.kApplet, "test", callback)
    finclip.finclip_register_callback_cpp(
        packer, finclip.kWebView, "test_webapi", PythonCallback().__disown__())
    finclip.finclip_initialize(packer)
    finclip.finclip_start_applet(1, appid)
    print("---------------------------------" + str(callback.thisown))


x = {
    "name": "John",
    "age": 30,
    "city": "New York"
}


def send_webapi():
    print(123)
    finclip.finclip_invoke_api_cpp(
        finclip.kWebView, appid, "test", json.dumps(x), PythonCallback().__disown__())


class Dialog(QDialog):
    NumGridRows = 3
    NumButtons = 4

    def __init__(self):
        super(Dialog, self).__init__()
        self.createFormGroupBox()

        buttonBox = QDialogButtonBox(
            QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        buttonBox.accepted.connect(self.accept)
        buttonBox.rejected.connect(self.reject)

        mainLayout = QVBoxLayout()
        mainLayout.addWidget(self.formGroupBox)
        # mainLayout.addWidget(buttonBox)
        self.setLayout(mainLayout)

        self.setWindowTitle("Form Layout - pythonspot.com")

    def createFormGroupBox(self):
        startBtn = QPushButton(self)
        startBtn.setText("start finclip")
        startBtn.clicked.connect(start_finclip)
        webapiBtn = QPushButton(self)
        webapiBtn.setText("invoke webapi")
        webapiBtn.clicked.connect(send_webapi)
        self.formGroupBox = QGroupBox("Form layout")
        layout = QFormLayout()
        layout.addRow(QLabel("Name:"), QLineEdit())
        layout.addRow(QLabel("Country:"), QComboBox())
        layout.addRow(QLabel("Age:"), QSpinBox())
        layout.addWidget(startBtn)
        layout.addWidget(webapiBtn)
        self.formGroupBox.setLayout(layout)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    dialog = Dialog()
    sys.exit(dialog.exec_())
