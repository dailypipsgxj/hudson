/*
* Copyright (C) 2007, Alberto Giannetti
*
* This file is part of Hudson.
*
* Hudson is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Hudson is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Hudson.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _SHORTPOSITION_HPP_
#define _SHORTPOSITION_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// Hudson
#include "Position.hpp"
#include "Price.hpp"


//! A new short position.
/*!
  ShortPosition automatically generates a new short Execution for a specific symbol/price/size. It runs a number of consistency checks to
  validate the price, date, and the specified execution ID. Only shorts() and cover() transactions can be executed, other transaction types
  will generate an exception.
*/
class ShortPosition: public Position
{
public:
  /*!
    All Execution parameters passed to the constructor are relative to the opening Execution.
    
    \param id A unique Execution ID.
    \param symbol Security for this position.
    \param dt Execution date.
    \param price Execution price.
    \param size Execution size.
    \see Execution.
  */
  ShortPosition(ID id, const std::string& symbol, const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);

  virtual Type type(void) const { return SHORT; }
  virtual std::string type_str(void) const { return "Short"; }

  //! The number of short executions run on this position.
  unsigned shorts(void) const { return _shorts; }
  //! The number of cover executions run on this position.
  unsigned covers(void) const { return _covers; }
  
  //! Always throws an exception. ShortPosition are not composite positions.
  //! \see StrategyPosition.
  virtual bool add(const PositionPtr pPos) throw(PositionException);
  
  //! Average short price.
  virtual double avgEntryPrice(void) const throw(PositionException) { return _avgShortPrice; }
  //! Average cover price.
  virtual double avgExitPrice(void) const throw(PositionException) { return _avgCoverPrice; }

  //! Current return factor: average short price divided by the average cover price.
  virtual double factor(void) const throw(PositionException);
  //! Return factor calculated dividing avgEntrPrice() into the price parameter.
  virtual double factor(const Price& price) const throw(PositionException);
  //! XXX: Should be a static.
  virtual double factor(const Price& prev_price, const Price& curr_price) const throw(PositionException);
  //! Return monthly factor for month/year period
  virtual double factor(const boost::gregorian::date::month_type& month, const boost::gregorian::date::year_type& year) const throw(PositionException);
  
  //! Throw an exception. ShortPosition can not be bought.
  virtual void buy(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  //! Throw an exception. ShortPosition can not be bought.
  virtual void buy(const boost::gregorian::date& dt, Series::EODDB::PriceType pt, unsigned size) throw(PositionException);
  
  //! Throw an exception. ShortPosition con not be sold.
  virtual void sell(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  //! Throw an exception. ShortPosition can not be sold.
  virtual void sell(const boost::gregorian::date& dt, Series::EODDB::PriceType pt, unsigned size) throw(PositionException);
  
  //! Add a ShortExecution.
  virtual void sell_short(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  //! Add a ShortExecution.
  virtual void sell_short(const boost::gregorian::date& dt, Series::EODDB::PriceType pt, unsigned size) throw(PositionException) { sell_short(dt, Price::get(_symbol, dt, pt), size); }
 
  //! Add a CoverExecution.
  virtual void cover(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  //! Add a CoverExecution.
  virtual void cover(const boost::gregorian::date& dt, Series::EODDB::PriceType pt, unsigned size) throw(PositionException) { cover(dt, Price::get(_symbol, dt, pt), size); }
  
  //! Close any open short size by adding a cover Execution.
  virtual void close(const boost::gregorian::date& dt, const Price& price) throw(PositionException);
  //! Close any open size on dt at market price.
  /*!
    \param dt The series date that will be used to retrieve a matching market price.
    \param pt The type of price that will be used to close the Position.
  */
  virtual void close(const boost::gregorian::date& dt, Series::EODDB::PriceType pt) throw(PositionException) { close(dt, Price::get(_symbol, dt, pt)); }
  
private:
  unsigned _shorts;
  unsigned _covers;

  double _avgShortPrice;
  double _avgCoverPrice;
};

#endif // _SHORTPOSITION_HPP_
