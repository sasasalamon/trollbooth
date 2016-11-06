class TrollboothController < ApplicationController
  def index
  end

  def open
    mqtt do |client|
      client.publish 'trollbooth', '1'
    end
  end

  private

  def mqtt
    client = MQTT::Client.new
    client.host = Rails.application.config.mqtt_host
    client.client_id = 'trollbooth_server'
    client.connect
    yield client
    client.disconnect
  end
end
