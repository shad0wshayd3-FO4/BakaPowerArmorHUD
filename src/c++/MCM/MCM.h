#pragma once

namespace MCM
{
	class Settings
	{
	public:
		class General
		{
		public:
			inline static bool bEnable{ true };

			inline static double fConditionMeterX{ 136.0 };
			inline static double fConditionMeterY{ 675.0 };
			inline static double fConditionMeterScale{ 1.0 };
		};

		static void Update()
		{
			ResetStateInit();

			GetModSettingBool("General", "bEnable", General::bEnable);

			GetModSettingDouble("General", "fConditionMeterX", General::fConditionMeterX);
			GetModSettingDouble("General", "fConditionMeterY", General::fConditionMeterY);
			GetModSettingDouble("General", "fConditionMeterScale", General::fConditionMeterScale);

			ResetStatePost();
		}

		inline static bool m_FirstRun{ true };

	private:
		static void ResetStateInit()
		{
			if (m_FirstRun)
			{
				m_FirstRun = false;
			}

			m_ini_base.LoadFile("Data/MCM/Config/BakaPowerArmorHUD/settings.ini");
			m_ini_user.LoadFile("Data/MCM/Settings/BakaPowerArmorHUD.ini");
		}

		static void ResetStatePost();

	protected:
		static void GetModSettingChar(const std::string& a_section, const std::string& a_setting, std::string_view& a_value)
		{
			auto base = m_ini_base.GetValue(a_section.c_str(), a_setting.c_str(), a_value.data());
			auto user = m_ini_user.GetValue(a_section.c_str(), a_setting.c_str(), base);
			a_value = user;
		}

		static void GetModSettingBool(const std::string& a_section, const std::string& a_setting, bool& a_value)
		{
			auto base = m_ini_base.GetBoolValue(a_section.c_str(), a_setting.c_str(), a_value);
			auto user = m_ini_user.GetBoolValue(a_section.c_str(), a_setting.c_str(), base);
			a_value = user;
		}

		static void GetModSettingDouble(const std::string& a_section, const std::string& a_setting, double& a_value)
		{
			auto base = m_ini_base.GetDoubleValue(a_section.c_str(), a_setting.c_str(), a_value);
			auto user = m_ini_user.GetDoubleValue(a_section.c_str(), a_setting.c_str(), base);
			a_value = user;
		}

		static void GetModSettingLong(const std::string& a_section, const std::string& a_setting, std::int32_t& a_value)
		{
			auto base = m_ini_base.GetLongValue(a_section.c_str(), a_setting.c_str(), a_value);
			auto user = m_ini_user.GetLongValue(a_section.c_str(), a_setting.c_str(), base);
			a_value = static_cast<std::int32_t>(user);
		}

		inline static CSimpleIniA m_ini_base{ true };
		inline static CSimpleIniA m_ini_user{ true };
	};
}
