#include <list>
#include <memory>
#include <vector>

enum class OrderType { GoodTillCancel, FillOrkill };

enum class Side { Buy, Sell };

using Price = std::int32_t;
using Quantity = std::uint32_t;
using OrderId = std::uint64_t;

struct LevelInfo {
  Price price_;
  Quantity quantity_;
};

using LevelInfos = std::vector<LevelInfo>;

class OrderBookLevelInfos {
public:
  OrderBookLevelInfos(const LevelInfo &bids, const LevelInfo &asks)
      : bids_{bids}, asks_{asks} {}

  const LevelInfos &GetBids() const { return bids_; }
  const LevelInfos &GetAsks() const { return asks_; }

private:
  LevelInfos bids_;
  LevelInfos asks_;
};

class Order {
  Order(OrderType orderType, OrderId orderId, Side side, Price price,
        Quantity quantity)
      : orderType_{orderType}, orderId_{orderId}, side_{side}, price_{price},
        initialQuantity_{quantity}, remainingQuantity_{quantity} {}

  OrderId GetOrderId() const { return orderId_; }
  Side GetSide() const { return side_; }
  Price GetPrice() const { return price_; }
  OrderType GetOrderType() const { return orderType_; }
  Quantity GetInitialQuantity() const { return initialQuantity_; }
  Quantity GetRemainingQuantity() const { return remainingQuantity_; }
  Quantity GetFilledQuantitiy() const {
    return GetInitialQuantity() - GetRemainingQuantity();
  }

  void Fill(Quantity quantity ) {
    if(quantity > GetRemainingQuantity()) {
      throw std::logic_error(std::format("Order ({}) cannot be filled for more than its remaining quantity", GetOrderId());
    }
    remainingQuantity_ -= quantity;
  }

private:
  OrderId orderId_;
  OrderType orderType_;
  Price price_;
  Side side_;
  Quantity initialQuantity_;
  Quantity remainingQuantity_;

};

using OrderPointer = std::shared_ptr<Order>;
using OrderPointers = std::list<OrderPointer>;

class OrderModify
{
public:
  OrderModify(OrderId orderId, Side side, Price price, Quantity quantity)
  : orderId_ { orderId }
  , price_ { price }
  , side_ { side }
  , quantity_ { quantity }
  {}
  OrderId GetOrderId() const { return orderId_; }
  Price GetPrice() const { return price_; }
  Side GetSide() const  { return side_; }
  Quantity GetQuantity() const { return quantity_; }

  OrderPointer ToOrderPointer(OrderType type) const
  {
    return std::make_shared<Order>(type, GetOrderId(), GetSide(), GetPrice(), GetQuantity());
  }

private:
  OrderId orderId_;
  Price price_;
  Side side_;
  Quantity quantity_;
};

struct TradeInfo
{
  OrderId orderId_;
  Price price_;
  Quantity quantity_;
};

class Trade
{
public:
  Trade(const TradeInfo& bidTrade, const TradeInfo& askTrade)
  : bidTrade_ {bidTrade}
  , askTrade_ { askTrade }
  {}

  const TradeInfo& GetBidTrade() const { return bidTrade_; }
  const TradeInfo& GetAskTrade() const { return askTrade_; }

private:
  TradeInfo bidTrade_;
  TradeInfo askTrade_;
};

using Trades = std::vector<Trade>;


int main() {
}
