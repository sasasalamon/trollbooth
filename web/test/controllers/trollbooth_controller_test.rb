require 'test_helper'

class TrollboothControllerTest < ActionDispatch::IntegrationTest
  test "should get index" do
    get trollbooth_index_url
    assert_response :success
  end

end
