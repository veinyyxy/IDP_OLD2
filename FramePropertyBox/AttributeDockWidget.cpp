#include "stdafx.h"
#include "AttributeDockWidget.h"

AttributeDockWidget::AttributeDockWidget(QWidget *parent)
	: QDockWidget(parent)
{
	ui.setupUi(this);

	m_pPropertyBrowser = new QtTreePropertyBrowser(this);
	m_pPropertyBrowser->setContentsMargins(0,0,0,0);
	this->setWidget(m_pPropertyBrowser);
}

AttributeDockWidget::~AttributeDockWidget()
{

}

/** 
  * @brief    添加属性   
  * @author   易家祥  
  * @param [in] parameter name   parameter description
  * @param [out] parameter name   parameter description
  * @return   AttributeDockWidget  添加属性
  * @note     易家祥
  * @date     2013年3月                               
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
void AttributeDockWidget::AddPropertyBrowser(FIELD_MAP layerAttributeDescripe, bool readonly /*= 0 说明是不能修改 1可以修改 */)
{
	QtProperty *item = NULL;
	std::map<int, GeomField>::iterator layerIter = layerAttributeDescripe.begin();
	while(layerIter != layerAttributeDescripe.end())
	{
		int index = layerIter->first;
		QVariant variant = layerIter->second.type();
		//////////////////////////////////////////////////////////////////////////
		std::string name = layerIter->second.name();
		std::string comment = layerIter->second.comment();
		boost::any _any = GetAttributeValue(index);

		// 判断属性类型：
		if(variant.toInt()== QVariant::Bool) // 布尔类型
		{
			QtBoolPropertyManager *boolManager = new QtBoolPropertyManager(this);
			QtCheckBoxFactory *checkBoxFactory = new QtCheckBoxFactory(this);
			item->setWhatsThis(QString::number(index));
			item->setToolTip(QString::fromStdString(comment));
			if (_any.type() == typeid(bool))
			{
				bool _value =  boost::any_cast<bool>(_any);
				boolManager->setValue(item,_value);
			}
			if (readonly)
			{
				m_pPropertyBrowser->setFactoryForManager(boolManager, checkBoxFactory);
			}
			connect(boolManager, SIGNAL(valueChanged(QtProperty *, bool)), this, SLOT(Slot_ValueChanged(QtProperty *, bool)));
		}
		else if((variant.toInt() == QVariant::Int) || (variant.toInt() == QVariant::UInt)) // 整形
		{
			QtIntPropertyManager *intManager = new QtIntPropertyManager(this);
			QtSpinBoxFactory *spinBoxFactory = new QtSpinBoxFactory(this);
			item = intManager->addProperty(QString::fromStdString(name));
			item->setWhatsThis(QString::number(index));
			item->setToolTip(QString::fromStdString(comment));
			if (readonly)
			{
				m_pPropertyBrowser->setFactoryForManager(intManager, spinBoxFactory);
			}

			if (_any.type() == typeid(int))
			{
				int _value =  boost::any_cast<int>(_any);
				//intManager->setRange(item,pAttr->Min(),pAttr->Max());
				intManager->setValue(item,_value);
			}
			else if (_any.type()  == typeid(uint))
			{
				int _value = (int)boost::any_cast<uint>(_any);
				intManager->setValue(item,_value);
			}
			
			connect(intManager, SIGNAL(valueChanged(QtProperty *, int)), this, SLOT(Slot_ValueChanged(QtProperty *, int)));
		} // ~~整形
		else if(variant.toInt() == QVariant::Double ) // 浮点型
		{
			QtDoublePropertyManager *doubleManager = new QtDoublePropertyManager(this);
			QtDoubleSpinBoxFactory* spinBoxFactory = new QtDoubleSpinBoxFactory(this);

			item = doubleManager->addProperty(QString::fromStdString(name));
			item->setWhatsThis(QString::number(index));
			item->setToolTip(QString::fromStdString(comment));
			if (_any.type() == typeid(double))
			{
				double _value =  boost::any_cast<double>(_any);
				//doubleManager->setDecimals(item, 6);
				//doubleManager->setSingleStep(item, 0.000001);
				//doubleManager->setRange(item,pAttr->DMin(),pAttr->DMax());
				doubleManager->setValue(item,_value);
			}
			if (readonly)
			{
				m_pPropertyBrowser->setFactoryForManager(doubleManager, spinBoxFactory);
			}
			connect(doubleManager, SIGNAL(valueChanged(QtProperty *, double)), this, SLOT(Slot_ValueChanged(QtProperty *, double)));
		}
		else if(variant.toInt() == QVariant::String) // 字符串
		{
			QtStringPropertyManager *stringManager = new QtStringPropertyManager(this);
			QtLineEditFactory *lineEditFactory = new QtLineEditFactory(this);
			item = stringManager->addProperty(QString::fromStdString(name));
			item->setWhatsThis(QString::number(index));
			item->setToolTip(QString::fromStdString(comment));
			if (_any.type() == typeid(std::string))
			{
				std::string _value =  boost::any_cast<std::string>(_any);
				stringManager->setValue(item,QString::fromStdString(_value));
			}
			if (readonly)
			{
				m_pPropertyBrowser->setFactoryForManager(stringManager, lineEditFactory);
			}
			connect(stringManager, SIGNAL(valueChanged(QtProperty *, const QString &)), this, SLOT(Slot_ValueChanged(QtProperty *, const QString &)));
		}
		else if(variant.toInt() == QVariant::UserType) //
		{
			//QtSizePolicyPropertyManager *sizePolicyManager = new QtSizePolicyPropertyManager(this);
			QtEnumPropertyManager *enumManager = new QtEnumPropertyManager(this);
			QtEnumEditorFactory *comboBoxFactory = new QtEnumEditorFactory(this);
			item = enumManager->addProperty(QString::fromStdString(name));
			item->setWhatsThis(QString::number(index));
			item->setToolTip(QString::fromStdString(comment));

			////////////////////////////////////////色卡设置Begin//////////////////////////////////
			if (_any.type() == typeid(osg::Vec4))
			{
				//enumManager->setEnumIcons(item,map);
				//enumManager->setEnumNames(item, enumNames);
				//qDebug()<< pAttr->Value().toInt();
				//enumManager->setValue(item, pAttr->Value().toInt());
			}
			////////////////////////////////////////色卡设置End////////////////////////////////////
			if (readonly)
			{
				m_pPropertyBrowser->setFactoryForManager(enumManager, comboBoxFactory);
			}
			connect(enumManager, SIGNAL(valueChanged(QtProperty *, int)), this, SLOT(Slot_ValueChanged(QtProperty *, int)));
		}
		else if(variant.toInt() == QVariant::Image) //
		{
			//QtSizePolicyPropertyManager *sizePolicyManager = new QtSizePolicyPropertyManager(this);
			QtEnumPropertyManager *enumManager = new QtEnumPropertyManager(this);
			QtEnumEditorFactory *comboBoxFactory = new QtEnumEditorFactory(this);
			item = enumManager->addProperty(QString::fromStdString(name));
			item->setWhatsThis(QString::number(index));
			item->setToolTip(QString::fromStdString(comment));
			////////////////////////////////////////色卡设置Begin//////////////////////////////////
			if (_any.type() == typeid(osg::Vec4))
			{
				//enumManager->setEnumIcons(item,map);
				//enumManager->setEnumNames(item, enumNames);
				//qDebug()<< pAttr->Value().toInt();
				//enumManager->setValue(item, pAttr->Value().toInt());
			}
			////////////////////////////////////////色卡设置End////////////////////////////////////
			if (readonly)
			{
				m_pPropertyBrowser->setFactoryForManager(enumManager, comboBoxFactory);
			}
			connect(enumManager, SIGNAL(valueChanged(QtProperty *, int)), this, SLOT(Slot_ValueChanged(QtProperty *, int)));
		}

		else if(variant.toInt() == QVariant::StringList) //
		{
			//QtSizePolicyPropertyManager *sizePolicyManager = new QtSizePolicyPropertyManager(this);
			QtEnumPropertyManager *enumManager = new QtEnumPropertyManager(this);
			QtEnumEditorFactory *comboBoxFactory = new QtEnumEditorFactory(this);
			item = enumManager->addProperty(QString::fromStdString(name));
			item->setWhatsThis(QString::number(index));
			item->setToolTip(QString::fromStdString(comment));
			if (_any.type() == typeid(QStringList))
			{
				QStringList _value =  boost::any_cast<QStringList>(_any);
				enumManager->setEnumNames(item, _value);
				enumManager->setValue(item, 0);
			}
			if (readonly)
			{
				m_pPropertyBrowser->setFactoryForManager(enumManager, comboBoxFactory);
			}
			connect(enumManager, SIGNAL(valueChanged(QtProperty *, int)), this, SLOT(Slot_ValueChanged(QtProperty *, int)));
		}
		if(variant.toInt() == QVariant::Font) // 字体
		{
			QtFontPropertyManager* fontManager = new QtFontPropertyManager(this);
			QtFontEditorFactory* fontEditFactory = new QtFontEditorFactory(this);
			QtSpinBoxFactory *spinBoxFactory = new QtSpinBoxFactory(this);
			QtCheckBoxFactory *checkBoxFactory = new QtCheckBoxFactory(this);
			item = fontManager->addProperty(QString::fromStdString(name));
			item->setWhatsThis(QString::number(index));
			item->setToolTip(QString::fromStdString(comment));
			if (_any.type() == typeid(std::string))
			{
				//fontManager->setValue(item, qVariantValue<QFont>(pAttr->Value()));
			}
			if (readonly)
			{
				m_pPropertyBrowser->setFactoryForManager(fontManager->subBoolPropertyManager(), checkBoxFactory);
				m_pPropertyBrowser->setFactoryForManager(fontManager->subIntPropertyManager(), spinBoxFactory);
				m_pPropertyBrowser->setFactoryForManager(fontManager, fontEditFactory);
			}
			connect(fontManager, SIGNAL(valueChanged(QtProperty *, const QFont &)), this, SLOT(Slot_ValueChanged(QtProperty *, const QFont &)));
		}
		else if(variant.toInt() == QVariant::Color) // 颜色
		{
			QtColorPropertyManager* colorManager = new QtColorPropertyManager(this);
			QtColorEditorFactory* colorEditFactory = new QtColorEditorFactory(this);
			QtSpinBoxFactory *spinBoxFactory = new QtSpinBoxFactory(this);
			item = colorManager->addProperty(QString::fromStdString(name));
			item->setWhatsThis(QString::number(index));
			item->setToolTip(QString::fromStdString(comment));

			if (_any.type() == typeid(osg::Vec4))
			{
				osg::Vec4 _value =  boost::any_cast<osg::Vec4>(_any);
				QColor color;
				color.setRedF(_value.r());
				color.setGreenF(_value.g());
				color.setBlueF(_value.b());
				color.setAlphaF(_value.a());
				colorManager->setValue(item, color);
			}
			if (readonly)
			{
				m_pPropertyBrowser->setFactoryForManager(colorManager->subIntPropertyManager(), spinBoxFactory);
				m_pPropertyBrowser->setFactoryForManager(colorManager, colorEditFactory);
			}
			connect(colorManager, SIGNAL(valueChanged(QtProperty *, const QColor &)), this, SLOT(Slot_ValueChanged(QtProperty *, const QColor &)));
		}
		else
		{/*
			QtStringPropertyManager *stringManager = new QtStringPropertyManager(this);
			QtLineEditFactory *lineEditFactory = new QtLineEditFactory(this);
			item = stringManager->addProperty(QString::fromStdString(name));
			item->setWhatsThis(QString::number(index));
			item->setToolTip(QString::fromStdString(comment));
			if (_any.type() == typeid(std::string))
			{
				std::string _value =  boost::any_cast<std::string>(_any);
				stringManager->setValue(item,QString::fromStdString(_value));
			}
			if (readonly)
			{
				m_pPropertyBrowser->setFactoryForManager(stringManager, lineEditFactory);
			}
			connect(stringManager, SIGNAL(valueChanged(QtProperty *, const QString &)), this, SLOT(Slot_ValueChanged(QtProperty *, const QString &)));
		*/
		}
		m_pPropertyBrowser->addProperty(item);
		////////////////////////////////////////////////////////////////////////
		layerIter++;
	}
}

//int 属性更改事件
void AttributeDockWidget::Slot_ValueChanged(QtProperty *property, int val)
{
	int iValue = val;//property->whatsThis().toInt();
		
	QString strName = property->propertyName();

	if (m_pLayer)
	{
		if(strName == "透明度")
		{
			m_pLayer->SetAlphaValue(iValue);
		}
		else if(strName == "线型")
		{ 
			if(val == 0)
				m_pLayer->SetLineStripple(1, 0xFFFF);
			else
				m_pLayer->SetLineStripple(1, 0x00FF);
		}
		if(strName == "线宽")
		{
			m_pLayer->SetLineWidth(iValue);
		}
	}

	m_pLayer->UpdateLayer();
}

//bool 属性更改事件
void AttributeDockWidget::Slot_ValueChanged(QtProperty *property, bool val)
{
	UpdateProperty(property,QVariant(val));
}

//double 属性更改事件
void AttributeDockWidget::Slot_ValueChanged(QtProperty *property, double val)
{
	QtProperty* pProperty = property;
	
	QString strName = pProperty->propertyName();
	
	if (m_pLayer)
	{
		if(strName == "线宽")
		{
			unsigned uLineWidth = pProperty->whatsThis().toUInt();
			m_pLayer->SetLineWidth(uLineWidth);
		}
		else if(strName == "点大小")
		{
			unsigned uPointSize = pProperty->whatsThis().toUInt();
			m_pLayer->SetPointSize(uPointSize);
		}
		m_pLayer->UpdateLayer();
	}
}

//QString 属性更改事件
void AttributeDockWidget::Slot_ValueChanged(QtProperty *property, const QString &val)
{
	UpdateProperty(property,QVariant(val));
}

//QColor 属性更改事件
void AttributeDockWidget::Slot_ValueChanged(QtProperty *property, const QColor &val)
{
	//UpdateProperty(property,QVariant(val));
	if (m_pLayer)
	{
		int index = property->whatsThis().toInt();
		osg::Vec4 vec4(val.redF(),val.greenF(),val.blueF(),val.alphaF());
		m_pLayer->SetLayerColor(vec4);
		m_pLayer->UpdateLayer();
		//m_pLayer->UpdateProperty(index,vec4);
	}
}

//QFont 属性更改事件
void AttributeDockWidget::Slot_ValueChanged(QtProperty *property, const QFont &val)
{
	UpdateProperty(property,QVariant(val));
}

//QPoint spinbox属性更改事件
void AttributeDockWidget::Slot_ValueChanged(QtProperty *property, const QPoint &val)
{
	UpdateProperty(property,QVariant(val));
}

//QSize spinbox属性更改事件
void AttributeDockWidget::Slot_ValueChanged(QtProperty *property, const QSize &val)
{
	UpdateProperty(property,QVariant(val));
}

//QStringList combox属性更改事件
void AttributeDockWidget::Slot_ValueChanged(QtProperty *property, const QStringList &val)
{
	if (m_pLayer)
	{
		int index = property->whatsThis().toInt();
		//m_pLayer->UpdateProperty(index,val);
	}
}

void AttributeDockWidget::UpdateProperty(QtProperty *property,QVariant& val)
{
	QtProperty* pProperty = property;

	QString strName = pProperty->propertyName();

	if (m_pLayer)
	{
		if(strName == "线宽")
		{
			unsigned uLineWidth = pProperty->whatsThis().toUInt();
			m_pLayer->SetLineWidth(uLineWidth);
		}
		else if(strName == "点大小")
		{
			unsigned uPointSize = pProperty->whatsThis().toUInt();
			m_pLayer->SetPointSize(uPointSize);
		}
		else if(strName == "透明度")
		{
			unsigned uPointSize = pProperty->whatsThis().toUInt();
			m_pLayer->SetAlphaValue(uPointSize);
		}
		m_pLayer->UpdateLayer();
	}
}

void AttributeDockWidget::ShowAttributePropertyBrowser(Layer* layer ,bool readonly)
{
	m_pPropertyBrowser->clear(); //先清空

	m_pLayer = layer;
	FIELD_MAP* layerAttributeDescripe = layer->GetLayerAttributeMap();
	AddPropertyBrowser(*layerAttributeDescripe,readonly);
	show();
}

boost::any AttributeDockWidget::GetAttributeValue(int index )
{
	boost::any _value;
	Layer::P_LAYER_ATTRIBUTE_MAP pLayerAttribteTable = m_pLayer->GetLayerAttribteTable();
	Layer::LAYER_ATTRIBUTE_MAP::iterator findResult = pLayerAttribteTable->find(index);
	_value = findResult->second;
	return _value;
	//Layer::LAYER_ATTRIBUTE_MAP::iterator layerIter = pLayerAttribteTable->begin();
	//while(layerIter != pLayerAttribteTable->end())
	//{
	//	if (layerIter->first == index)
	//	{
	//		_value = layerIter->second;
	//		return _value;
	//	}
	//	layerIter++;
	//}
	return NULL;
}

void AttributeDockWidget::setVisible( bool visible )
{
	//if (visible)
	{
		QAction* action = (QAction*)this->property("Action").toLongLong();
		if (action)
		{
			action->setCheckable(visible);
			action->setChecked(visible);
		}
	}
	QDockWidget::setVisible(visible);
}
