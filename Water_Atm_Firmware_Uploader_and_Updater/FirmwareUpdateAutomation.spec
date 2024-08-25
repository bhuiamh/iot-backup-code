# -*- mode: python ; coding: utf-8 -*-

block_cipher = None


a = Analysis(['FirmwareUpdateAutomation.py'],
             pathex=['/home/amiriot/Desktop/Office_Intern/water atm/MQTT_FU_V3.3_with_fail_safe/water_atm_firmware/Firmware_Sender/optimization_test'],
             binaries=[],
             datas=[],
             hiddenimports=[],
             hookspath=[],
             runtime_hooks=[],
             excludes=[],
             win_no_prefer_redirects=False,
             win_private_assemblies=False,
             cipher=block_cipher,
             noarchive=False)
pyz = PYZ(a.pure, a.zipped_data,
             cipher=block_cipher)
exe = EXE(pyz,
          a.scripts,
          a.binaries,
          a.zipfiles,
          a.datas,
          [],
          name='FirmwareUpdateAutomation',
          debug=False,
          bootloader_ignore_signals=False,
          strip=False,
          upx=True,
          upx_exclude=[],
          runtime_tmpdir=None,
          console=True , icon='FUA.icon')
