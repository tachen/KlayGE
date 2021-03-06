// ShowFactory.hpp
// KlayGE 播放引擎抽象工厂 头文件
// Ver 3.4.0
// 版权所有(C) 龚敏敏, 2006
// Homepage: http://www.klayge.org
//
// 3.4.0
// 初次建立 (2006.7.15)
//
// 修改记录
/////////////////////////////////////////////////////////////////////////////////

#ifndef _SHOWFACTORY_HPP
#define _SHOWFACTORY_HPP

#pragma once

#include <KlayGE/PreDeclare.hpp>

#include <string>
#include <boost/noncopyable.hpp>

namespace KlayGE
{
	class KLAYGE_CORE_API ShowFactory
	{
	public:
		virtual ~ShowFactory()
		{
		}

		static ShowFactoryPtr NullObject();

		virtual std::wstring const & Name() const = 0;
		ShowEngine& ShowEngineInstance();

		void Suspend();
		void Resume();

	private:
		virtual ShowEnginePtr MakeShowEngine() = 0;
		virtual void DoSuspend() = 0;
		virtual void DoResume() = 0;

	protected:
		ShowEnginePtr se_;
	};

	template <typename ShowEngineType>
	class ConcreteShowFactory : boost::noncopyable, public ShowFactory
	{
	public:
		ConcreteShowFactory(std::wstring const & name)
				: name_(name)
			{ }

		std::wstring const & Name() const
			{ return name_; }

	private:
		ShowEnginePtr MakeShowEngine()
		{
			return MakeSharedPtr<ShowEngineType>();
		}

		virtual void DoSuspend() KLAYGE_OVERRIDE
		{
		}
		virtual void DoResume() KLAYGE_OVERRIDE
		{
		}

	private:
		std::wstring const name_;
	};
}

#endif			// _SHOWFACTORY_HPP
