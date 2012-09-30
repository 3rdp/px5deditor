/*
	Korg Pandora PX5D Editor for Linux
	Copyright (C) 2011-2012, Frederic Jaume <dev@exp-media.com>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>
#include <QMessageBox>

#include "engine/presetparameters.h"
#include "engine/presetdynamics.h"
#include "engine/presetamp.h"

#include "aboutdialog.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	m_dlgAbout = 0;

	ui->setupUi(this);

	setWindowTitle(QString("Pandora PX5D Editor for Linux - v0.1 alpha"));

	m_px5dController = new Px5dController();
	m_px5dController->addObserver(*this);

	// Fill combo box elements
	for ( int i=PresetDynamics::COMPRESSOR; i<PresetDynamics::END; ++i ) {
		ui->dynamicsType->addItem( QString( PresetDynamics::Effects[i].fullName.c_str() ) );
	}

	for ( int i=PresetAmp::BTQCLN; i<PresetAmp::END; ++i ) {
		ui->ampType->addItem( QString( PresetAmp::Amps[i].fullName.c_str() ) );
	}

	for ( int i=PresetCabinet::TWEED1x8; i<PresetCabinet::END; ++i ) {
		ui->cabinetType->addItem( QString( PresetCabinet::Cabs[i].fullName.c_str() ) );
	}

	for ( int i=PresetModulation::CHORUS1; i<PresetModulation::END; ++i ) {
		ui->modulationType->addItem( QString( PresetModulation::Modulations[i].fullName.c_str() ) );
	}

	for ( int i=PresetDelay::SLAP1; i<PresetDelay::END; ++i ) {
		ui->delayType->addItem( QString( PresetDelay::Delays[i].fullName.c_str() ) );
	}

	for ( int i=PresetReverb::DRYAIR; i<PresetReverb::END; ++i ) {
		ui->reverbType->addItem( QString( PresetReverb::Reverbs[i].fullName.c_str() ) );
	}

	//ui->noiseReductionParamName->setText( QString( m_px5dController->Preset()->noiseReduction.getParamName()) );

	// FIXME: set wheelScrollLines(1) for all Dials !!!
}

MainWindow::~MainWindow()
{
	delete m_px5dController;
	delete ui;
}

void MainWindow::update(PandoraObservable* o, PandoraUpdatedSignal bitflag, PandoraPreset &p) {

	if ( bitflag==PX5D_HANDSHAKE ) {
		//TODO
	}

	if ( bitflag==PX5D_ACK ) {
		//TODO
	}

	if ( bitflag==PX5D_UPDATE_ALL ) {
		ui->programName->setText( QString(p.name.c_str()) );
		ui->programNumber->setValue(p.number);

		if ( p.slot==PandoraPreset::PROGRAM_FACTORY ) {
			ui->programBankFactory->setChecked(true);
		}
		else if ( p.slot==PandoraPreset::PROGRAM_USER ) {
			ui->programBankUser->setChecked(true);
		}
	}

	if ( bitflag&PX5D_UPDATE_DYN || bitflag==PX5D_UPDATE_ALL ) {
#ifdef DEBUG
		std::cout << " got dyn update call " << std::endl;
#endif
		ui->dynamicsOn->setChecked(p.dynamics.getEnabled());

		ui->dynamicsType->setCurrentIndex( p.dynamics.getEffect() );
		ui->dynamicsParamName->setText( QString( p.dynamics.getParamName() ) );
		ui->dynamicsParamDial->setMinimum( p.dynamics.getMinParam() );
		ui->dynamicsParamDial->setMaximum( p.dynamics.getMaxParam() );
		ui->dynamicsParamDial->setValue( p.dynamics.getParameter() );

		//ui->dynamicsParam will auto change as we change the dial param

	}

	if ( bitflag&PX5D_UPDATE_AMP|| bitflag==PX5D_UPDATE_ALL ) {

#ifdef DEBUG
		std::cout << " got amp update call " << std::endl;
#endif
		ui->amplifierOn->setChecked(p.amp.getEnabled());
		ui->ampType->setCurrentIndex( p.amp.getModel() );

		//Gain, Bass, Treble, Volume, Middle
		int min	= p.amp.getMinParam(p.amp.getModel(), PresetAmp::AMP_GAIN);
		int max	= p.amp.getMaxParam(p.amp.getModel(), PresetAmp::AMP_GAIN);

		ui->ampParamName1->setText( QString( p.amp.getParamName(p.amp.getModel(), PresetAmp::AMP_GAIN) ) );
		ui->ampParamDial1->setMinimum( min );
		ui->ampParamDial1->setMaximum( max );
		ui->ampParamDial1->setValue( p.amp.getGain() );

		min	= p.amp.getMinParam(p.amp.getModel(), PresetAmp::AMP_BASS);
		max	= p.amp.getMaxParam(p.amp.getModel(), PresetAmp::AMP_BASS);
		ui->ampParamName2->setText( QString( p.amp.getParamName(p.amp.getModel(), PresetAmp::AMP_BASS) ) );
		ui->ampParamDial2->setMinimum( min );
		ui->ampParamDial2->setMaximum( max );
		ui->ampParamDial2->setValue( p.amp.getBass() );

		min	= p.amp.getMinParam(p.amp.getModel(), PresetAmp::AMP_TREBLE);
		max	= p.amp.getMaxParam(p.amp.getModel(), PresetAmp::AMP_TREBLE);
		ui->ampParamName3->setText( QString( p.amp.getParamName(p.amp.getModel(), PresetAmp::AMP_TREBLE) ) );
		ui->ampParamDial3->setMinimum( min );
		ui->ampParamDial3->setMaximum( max );
		ui->ampParamDial3->setValue( p.amp.getTreble() );

		min	= p.amp.getMinParam(p.amp.getModel(), PresetAmp::AMP_VOLUME);
		max	= p.amp.getMaxParam(p.amp.getModel(), PresetAmp::AMP_VOLUME);
		ui->ampParamName4->setText( QString( p.amp.getParamName(p.amp.getModel(), PresetAmp::AMP_VOLUME) ) );
		ui->ampParamDial4->setMinimum( min );
		ui->ampParamDial4->setMaximum( max );
		ui->ampParamDial4->setValue( p.amp.getVolume() );

		min	= p.amp.getMinParam(p.amp.getModel(), PresetAmp::AMP_MIDDLE);
		max	= p.amp.getMaxParam(p.amp.getModel(), PresetAmp::AMP_MIDDLE);
		ui->ampParamName5->setText( QString( p.amp.getParamName(p.amp.getModel(), PresetAmp::AMP_MIDDLE) ) );
		ui->ampParamDial5->setMinimum( min );
		ui->ampParamDial5->setMaximum( max );
		ui->ampParamDial5->setValue( p.amp.getMiddle() );

	}

	if ( bitflag&PX5D_UPDATE_CAB|| bitflag==PX5D_UPDATE_ALL ) {
#ifdef DEBUG
		std::cout << " got cab update call " << std::endl;
#endif
		ui->cabinetOn->setChecked(p.cabinet.getEnabled());

		ui->cabinetType->setCurrentIndex( p.cabinet.getCabinet() );
		ui->cabinetParamName->setText( QString( p.cabinet.getParamName() ) );
		ui->cabinetParamDial->setMinimum( p.cabinet.getMinParam() );
		ui->cabinetParamDial->setMaximum( p.cabinet.getMaxParam() );
		ui->cabinetParamDial->setValue( p.cabinet.getParameter() );
	}

	if ( bitflag&PX5D_UPDATE_MOD|| bitflag==PX5D_UPDATE_ALL ) {

#ifdef DEBUG
		std::cout << " got mod update call " << std::endl;
#endif
		ui->modulationOn->setChecked(p.modulation.getEnabled());

		ui->modulationType->setCurrentIndex( p.modulation.getModulation() );
		ui->modulationParamName->setText( QString( p.modulation.getParamName() ) );
		ui->modulationParamDial->setMinimum( p.modulation.getMinParam() );
		ui->modulationParamDial->setMaximum( p.modulation.getMaxParam() );
		ui->modulationParamDial->setValue( p.modulation.getParameter() );

	}

	if ( bitflag&PX5D_UPDATE_DELAY|| bitflag==PX5D_UPDATE_ALL ) {
#ifdef DEBUG
		std::cout << " got delay update call " << std::endl;
#endif
		ui->delayOn->setChecked(p.delay.getEnabled());

		ui->delayType->setCurrentIndex( p.delay.getDelay() );
		ui->delayParamName->setText( QString( p.delay.getParam1Name() ) );
		ui->delayParamDial->setMinimum( p.delay.getMinParam1() );
		ui->delayParamDial->setMaximum( p.delay.getMaxParam1() );
		ui->delayParamDial->setValue( p.delay.getParameter1() );

		ui->delayParamName2->setText( QString( p.delay.getParam2Name() ) );
		ui->delayParamDial2->setMinimum( p.delay.getMinParam2() );
		ui->delayParamDial2->setMaximum( p.delay.getMaxParam2() );
		ui->delayParamDial2->setValue( p.delay.getParameter2() );
	}

	if ( bitflag&PX5D_UPDATE_REVERB|| bitflag==PX5D_UPDATE_ALL ) {

#ifdef DEBUG
		std::cout << " got reverb update call " << std::endl;
#endif
		ui->reverbOn->setChecked(p.reverb.getEnabled());

		ui->reverbType->setCurrentIndex( p.reverb.getReverb() );
		ui->reverbParamName->setText( QString( p.reverb.getParamName() ) );
		ui->reverbParamDial->setMinimum( p.reverb.getMinParam() );
		ui->reverbParamDial->setMaximum( p.reverb.getMaxParam() );
		ui->reverbParamDial->setValue( p.reverb.getParameter() );

	}

	if ( bitflag&PX5D_UPDATE_NOISE|| bitflag==PX5D_UPDATE_ALL ) {

#ifdef DEBUG
		std::cout << " got noise reduction update call " << std::endl;
#endif
		ui->noiseReductionOn->setChecked(p.noiseReduction.getEnabled());

		ui->noiseReductionParamName->setText( QString( p.noiseReduction.getParamName() ) );
		ui->noiseReductionParamDial->setMinimum( p.noiseReduction.getMinParam() );
		ui->noiseReductionParamDial->setMaximum( p.noiseReduction.getMaxParam() );
		ui->noiseReductionParamDial->setValue( p.noiseReduction.getParameter() );

	}

}

void MainWindow::on_connectToPandora_released()
{
	if ( m_px5dController->connectToPandora() ) {
		ui->px5dStatusIn->setText( QString(m_px5dController->pandoraInPortName().c_str()) );
		ui->px5dStatusOut->setText( QString(m_px5dController->pandoraOutPortName().c_str()) );

	} else {
		ui->px5dStatusIn->setText( tr("Cannot connect to Pandora...") );
		ui->px5dStatusOut->setText( tr("Cannot connect to Pandora...") );
	}

}

void MainWindow::on_programNumber_valueChanged(int v) {
	m_px5dController->setProgramNumber(v, ui->programBankFactory->isChecked()?PandoraPreset::PROGRAM_FACTORY : PandoraPreset::PROGRAM_USER);
}

void MainWindow::on_dynamicsOn_stateChanged(int v) {
	m_px5dController->setModuleState( PXD5_MODULE_DYN, v!=Qt::Unchecked );
}

void MainWindow::on_amplifierOn_stateChanged(int v) {
	m_px5dController->setModuleState( PXD5_MODULE_AMP, v!=Qt::Unchecked );
}

void MainWindow::on_cabinetOn_stateChanged(int v) {
	m_px5dController->setModuleState( PXD5_MODULE_CAB, v!=Qt::Unchecked );
}

void MainWindow::on_modulationOn_stateChanged(int v) {
	m_px5dController->setModuleState( PXD5_MODULE_MOD, v!=Qt::Unchecked );
}

void MainWindow::on_delayOn_stateChanged(int v) {
	m_px5dController->setModuleState( PXD5_MODULE_DELAY, v!=Qt::Unchecked );
}

void MainWindow::on_reverbOn_stateChanged(int v) {
	m_px5dController->setModuleState( PXD5_MODULE_REVERB, v!=Qt::Unchecked );
}

void MainWindow::on_dynamicsParamDial_valueChanged(int v) {
	m_px5dController->setParamChanged( PXD5_MODULE_DYN_PARAM, v );
	const char* paramValueAsText = m_px5dController->Preset()->dynamics.getParamStringValue(v);
	ui->dynamicsParam->setText(QString(paramValueAsText));
}

void MainWindow::on_dynamicsType_activated(int index) {
	m_px5dController->setParamChanged( PXD5_MODULE_DYN_TYPE, index );
}

void MainWindow::on_programBankFactory_clicked() {
	m_px5dController->setProgramNumber( ui->programNumber->value(), PandoraPreset::PROGRAM_FACTORY );
}

void MainWindow::on_programBankUser_clicked() {
	m_px5dController->setProgramNumber( ui->programNumber->value(), PandoraPreset::PROGRAM_USER );
}

void MainWindow::on_ampType_activated(int v) {
	m_px5dController->setParamChanged( PXD5_MODULE_AMP_TYPE, v );
}

void MainWindow::on_ampParamDial1_valueChanged(int v) {
	m_px5dController->setParamChanged( PXD5_MODULE_AMP_PARAM1, v );

	const char* paramValueAsText = m_px5dController->Preset()->amp.getParamStringValue(PresetAmp::AMP_GAIN, v);
	ui->ampParam1->setText(QString(paramValueAsText));
}

void MainWindow::on_ampParamDial2_valueChanged(int v) {
	m_px5dController->setParamChanged( PXD5_MODULE_AMP_PARAM2, v );

	const char* paramValueAsText = m_px5dController->Preset()->amp.getParamStringValue(PresetAmp::AMP_BASS, v);
	ui->ampParam2->setText(QString(paramValueAsText));
}

void MainWindow::on_ampParamDial3_valueChanged(int v) {
	m_px5dController->setParamChanged( PXD5_MODULE_AMP_PARAM3, v );
	const char* paramValueAsText = m_px5dController->Preset()->amp.getParamStringValue(PresetAmp::AMP_TREBLE, v);
	ui->ampParam3->setText(QString(paramValueAsText));
}

void MainWindow::on_ampParamDial4_valueChanged(int v) {
	m_px5dController->setParamChanged( PXD5_MODULE_AMP_PARAM4, v );
	const char* paramValueAsText = m_px5dController->Preset()->amp.getParamStringValue(PresetAmp::AMP_VOLUME, v);
	ui->ampParam4->setText(QString(paramValueAsText));
}

void MainWindow::on_ampParamDial5_valueChanged(int v) {
	m_px5dController->setParamChanged( PXD5_MODULE_AMP_PARAM5, v );
	const char* paramValueAsText = m_px5dController->Preset()->amp.getParamStringValue(PresetAmp::AMP_MIDDLE, v);
	ui->ampParam5->setText(QString(paramValueAsText));
}

void MainWindow::on_cabinetType_activated(int v) {
	m_px5dController->setParamChanged( PXD5_MODULE_CAB_TYPE, v );
}

void MainWindow::on_cabinetParamDial_valueChanged(int v) {
	m_px5dController->setParamChanged( PXD5_MODULE_CAB_PARAM, v );
	const char* paramValueAsText = m_px5dController->Preset()->cabinet.getParamStringValue(v);
	ui->cabinetParam->setText(QString(paramValueAsText));
}

void MainWindow::on_modulationType_activated(int v) {
	m_px5dController->setParamChanged( PXD5_MODULE_MOD_TYPE, v );
}

void MainWindow::on_modulationParamDial_valueChanged(int v) {
	m_px5dController->setParamChanged( PXD5_MODULE_MOD_PARAM, v );
	const char* paramValueAsText = m_px5dController->Preset()->modulation.getParamStringValue(v);
	ui->modulationParam->setText(QString(paramValueAsText));
}

void MainWindow::on_delayType_activated(int v) {
	m_px5dController->setParamChanged( PXD5_MODULE_DELAY_TYPE, v );
}

void MainWindow::on_delayParamDial_valueChanged(int v) {
	m_px5dController->setParamChanged( PXD5_MODULE_DELAY_PARAM1, v );
	const char* paramValueAsText = m_px5dController->Preset()->delay.getParam1StringValue(v);
	ui->delayParam->setText(QString(paramValueAsText));
}

void MainWindow::on_delayParamDial2_valueChanged(int v) {
	m_px5dController->setParamChanged( PXD5_MODULE_DELAY_PARAM2, v );
	const char* paramValueAsText = m_px5dController->Preset()->delay.getParam2StringValue(v);
	ui->delayParam2->setText(QString(paramValueAsText));
}

void MainWindow::on_reverbType_activated(int v) {
	m_px5dController->setParamChanged( PXD5_MODULE_REVERB_TYPE, v );
}


void MainWindow::on_reverbParamDial_valueChanged(int v) {
	m_px5dController->setParamChanged( PXD5_MODULE_REVERB_PARAM, v );
	const char* paramValueAsText = m_px5dController->Preset()->reverb.getParamStringValue(v);
	ui->reverbParam->setText(QString(paramValueAsText));
}

void MainWindow::on_noiseReductionParamDial_valueChanged(int v) {
	m_px5dController->setParamChanged( PXD5_MODULE_NOISE_REDUCTION, v );
	const char* paramValueAsText = m_px5dController->Preset()->noiseReduction.getParamStringValue(v);
	ui->noiseReductionParam->setText(QString(paramValueAsText));
	ui->noiseReductionOn->setChecked(v>0);
}

void MainWindow::on_noiseReductionOn_stateChanged(int v) {

	// Noise reduction is not a real enabled/disabled module.
	// It's just disabled when level is at 0, enabled otherwise.
	// When enabling, let's put it on 1 (0x03 in sysex value)
	ui->noiseReductionParamDial->setValue( v==Qt::Unchecked?0:3 );
}

void MainWindow::on_writeProgram_released() {

	//TODO: implement selecting destination program via dedicated dialog

	if ( ui->programBankFactory->isChecked() ) {

		QMessageBox msgBox;
		msgBox.setText("Save Program to Pandora");
		msgBox.setInformativeText("This program is a Factory program and will be saved to the User progam of same number. Is this ok ?");
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Save);
		int ret = msgBox.exec();

		switch (ret) {
		case QMessageBox::Save:
			m_px5dController->SaveCurrentProgramToSlot( ui->programNumber->value() );
			break;
		case QMessageBox::Cancel:
			break;
		default:
			break;
		}

	} else {

		QMessageBox msgBox;
		msgBox.setText("Save Program to Pandora");
		msgBox.setInformativeText("This program will overwrite the user progam of same number. Is this ok ?");
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Save);
		int ret = msgBox.exec();

		switch (ret) {
		case QMessageBox::Save:
			m_px5dController->SaveCurrentProgramToSlot( ui->programNumber->value() );
			break;
		case QMessageBox::Cancel:
			break;
		default:
			break;
		}

	}
}

void MainWindow::on_actionQuit_triggered() {
	exit(0);
}

void MainWindow::on_actionAbout_triggered() {
	about()->exec();
}

AboutDialog* MainWindow::about() {
	if (m_dlgAbout == 0) {
		m_dlgAbout = new AboutDialog(this);
	}
	return m_dlgAbout;
}
